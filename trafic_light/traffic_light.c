#include <avr/interrupt.h>
#include <avr/io.h>

#include "light_control.h"
#include "ports_and_bits.h"


char direction = 'n';
char ns_pedestrian_pressed = 0;
char ew_pedestrian_pressed = 0;
char currentOp = -1;
char nextOpReady = 0;

void setup() {
  // Bit 3 and 2 are interrupt input
  PORT_D_DDR = 0b11110011;
  PORT_B_DDR = 0b111111;
  // Start all directions red
  PORT_B |= NS_RED_CAR | EW_RED_CAR;
  PORT_D |= NS_RED_PEDSTRIAN | EW_RED_PEDSTRIAN;

  // TCCR1B: Timer/Counter Control Register B for the 16 bit timer
  // CS12 + CS 10: Set count source to clk I/O/1024 (From prescaler)
  // WGM12 = CTC (clear on compare match)
  // Page 143 in docs
  TCCR1B |= (_BV(CS12) | _BV(CS10) | _BV(WGM12));
  // Compare register for 16bit counter A

  // MAX VALUE IS 65536 - anything higher and it overflows
  OCR1A = MAX_CYCLES;
  // OCR1B = 3 * TICKS_PER_SECOND;
  // Enable interrupts from compare match A
  TIMSK1 = _BV(OCIE1A);

  // Enable INT0 and INT1 in EIMSK
  // EIMSK determines which external interrupts are enabled
  EIMSK = _BV(INT0) | _BV(INT1);

  // EICRA determines the trigger for the interrupts
  // Set EICRA to falling on INT0 and INT1
  EICRA = _BV(ISC01) | _BV(ISC11);

  sei(); // Enable external interrupt
  // An alternative is to write to SREG (0x5f) directly
  // SREG |= _BV(SREG_I) (the I bit is the 7th bit);
}

// Interrupt from NS button being pressed
ISR(INT0_vect) {
  if (direction == 'w') {
    ns_pedestrian_pressed = 1;
  }
}

// Interrupt from EW button being pressed
ISR(INT1_vect) {
  if (direction == 'n') {
    ew_pedestrian_pressed = 1;
  }
}

// Interrupt used in traffic light
ISR(TIMER1_COMPA_vect) {
  set_next_stage(1); // Used in light control
  nextOpReady = 1; // Used in loop()
}


int set_timer(double seconds) {
  unsigned long clock_cycles = seconds * TICKS_PER_SECOND;
  if (clock_cycles > MAX_CYCLES) {
    clock_cycles = MAX_CYCLES;
  }
  // Set the compare register to the number of clock cycles
  OCR1A = clock_cycles;
  // Then reset the counter
  TCNT1 = 0;
  // Return 0 since we want to pause the loop whenever set_timer is called.
  // We can then use the 0 to set a variable value
  return 0;
}

void loop() {
  if (nextOpReady) {
    // Reset the "next stage" before every operation
    set_next_stage(0);
    // Increment currentOp before each operation
    currentOp++;
    // Ready will be set to 0 every time set_timer is called, and set back to 1
    // every time the interrupt is then reached.
    switch (currentOp) {
      case 0:
        ns_to_green();
        nextOpReady = set_timer(1);
        break;
      case 1:
        ns_pedestrian_to_green();
        // ns_pedestrian_pressed = 0;
        direction = 'n';
        nextOpReady = set_timer(5);
        break;
      case 2:
        ns_pedestrian_to_red();
        nextOpReady = set_timer(1);
        // ready = set_timer(0.5);
        break;
      case 3:
        ns_to_red();
        set_next_stage(0);
        ew_to_green();
        nextOpReady = set_timer(1);
        // ready = set_timer(0.3);
        break;
      case 4:
        ew_pedestrian_to_green();
        // ew_pedestrian_pressed = 0;
        direction = 'w';
        nextOpReady = set_timer(5);
        break;
      case 5:
        ew_pedestrian_to_red();
        nextOpReady = set_timer(1);
        // ready = set_timer(0.5);
        break;
      case 6:
        ew_to_red();
        // We manually set the currentOp back to the beginning op -1 since it
        // auto increments at the start of the switch
        currentOp = -1;
        break;
    }

  }
  else if (ns_pedestrian_pressed) {
    // Turn on the light indicating that the button has been pressed
    PORTD |= NS_BUTTON_PRESSED;
    // We only want the "preemptive interrupt" to occur if we're in the long
    // wait between NS/EW
    if (currentOp == 4) {
      unsigned long currentCounter = TCNT1;
      OCR1A = currentCounter + (0.1 * TICKS_PER_SECOND);
      // Make sure this is only called once
      ns_pedestrian_pressed = 0;
      // And make sure that the button can't be pressed again until next iteration
      direction = 0;
    } 
  }
  // And do the same for the other direction
  else if (ew_pedestrian_pressed) {
    PORTD |= EW_BUTTON_PRESSED;
    if (currentOp == 1) { // Nice and hard coded
      unsigned long currentCounter = TCNT1;
      OCR1A = currentCounter + (1 * TICKS_PER_SECOND);
      ew_pedestrian_pressed = 0;
      direction = 0;
    }
  }
}

// Debugging function
void everything_on() {
  PORTD = 0b11110011;
  PORTB = 0b111111;
}

int main() {
  setup();
  // Small delay before we start
  set_timer(1);
  while (1) {
    loop();
  }

  return 0;
}
