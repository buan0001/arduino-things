#include <avr/interrupt.h>
#include <avr/io.h>

#include "light_control.h"
#include "ports_and_bits.h"


// unsigned long CLOCK_SPEED = 16000000;
// unsigned long TICKS_PER_SECOND = CLOCK_SPEED / 1024;
char direction = 'n';
char ns_pedestrian_pressed = 0;
char ew_pedestrian_pressed = 0;

void interruptable_delay(volatile long time) {
  time *= 1000;
  // If time is bigger than min_time and the button is pressed in the correct
  // direction - delay with min_time instead
  long min_time = time * 0.2;

  while (time) {
    if (direction == 'n' && ew_pedestrian_pressed) {
      // Turn on the light indicating that the button has been pressed
      PORT_D |= NS_BUTTON_PRESSED;
      delay(min_time);
      break;
    } else if (direction == 'w' && ns_pedestrian_pressed) {
      PORTD |= EW_BUTTON_PRESSED;
      delay(min_time);
      break;
    }
  }
}

void setup() {
  // Bit 2 and 3 are interrupt input
  PORT_D_DDR = 0b11110011;
  PORT_B_DDR |= 0b111111;
  // Start with both red
  // PORT_B |= NS_RED_CAR | EW_RED_CAR;
  // PORT_D |= NS_RED_PEDSTRIAN | EW_RED_PEDSTRIAN;
  // PORTB |= _BV(0) | _BV(1) | _BV(2) | _BV(3);

  PORTD |= (_BV(5) | _BV(6));

  // TCCR1B: Timer/Counter Control Register B for the 16 bit timer
  // CS12 + CS 10: Set count source to clk I/O/1024 (From prescaler)
  // WGM12 = CTC (clear on compare match)
  // Page 143 in docs
  TCCR1B |= (_BV(CS12) | _BV(CS10) | _BV(WGM12));
  // Compare register for 16bit counter A
  
  // MAX VALUE IS 65536 - anything higher and it overflows
  OCR1A = 3 * TICKS_PER_SECOND;
  // Enable interrupts from compare match A
  TIMSK1 = _BV(OCIE1A);
  // Enable external interrupt
  sei();
  // An alternative is to write to SREG (0x5f) directly
  // SREG |= _BV(SREG_I) (the I bit is the 7th bit);
}

// Interrupt from NS button being pressed
ISR(INT0_vect) { ns_pedestrian_pressed = 1; }

// Interrupt from EW button being pressed
ISR(INT1_vect) { ew_pedestrian_pressed = 1; }

// Turn off bit 5 on port d when the timer matches the compare register
// ISR(TIMER1_COMPA_vect) { PORTD &= ~(_BV(5)); }
ISR(TIMER1_COMPA_vect) { PORTD ^= _BV(5); }

char currentOp = 0;
void loop() {
  switch (currentOp){
    case 0:
    direction = 'n';
    
    break;
    case 1: break;
    case 2: break;
  }
    direction = 'n';
    ns_to_green();

    delay(300);

    ns_pedestrian_to_green();
    // If the button was pressed - turn it off again
    PORT_D &= ~(NS_BUTTON_PRESSED);

    delay(1500);

    ns_pedestrian_to_red();
    delay(500);
    ns_to_red();
    // Small delay between one side turning red and the other turning green
    // Change direction
    direction = 'w';

    ew_to_green();
    delay(300);

    // Turn off pedestrian light if it was on
    PORT_D &= ~(EW_BUTTON_PRESSED);
    ew_pedestrian_to_green();

    delay(1500);

    ew_pedestrian_to_red();
    delay(500);

    // if (PORT_B & EW_BUTTON_PRESSED) {
    //   PORT_D |= EW_BUTTON_PRESSED | NS_BUTTON_PRESSED;
    // } else {
    //   PORT_D &= ~(EW_BUTTON_PRESSED | NS_BUTTON_PRESSED);
    // }

    ew_to_red();
}

int main() {
  setup();

  delay(500);
  while (1){
    loop();
  }

  return 0;
}
