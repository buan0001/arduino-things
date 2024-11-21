#include <avr/io.h>
// #include <avr/iom328p.h>
#include <avr/interrupt.h>

void increment() { PORTB ^= _BV(0); }

void wait() {
  for (volatile long i = 0; i < 1000000; i++) {
  }
}

void loop() {
  // static unsigned char count = 0;
  // turn of upper _BVs - then set them again with the counter
  PORTB = (PORTD & 0x0f) | (TCNT0 << 4);
  // PORTD = (PORTD & 0x0f) | (count << 4);
  // // increment counter, completely forget about the upper 4 _BVs
  // count++;

  wait();
}

void setup() {
  // Set PD4,5,6,7 as output
  DDRD |= (_BV(4) | _BV(5) | _BV(6) | _BV(7));

  // Set PD2 as input (kind of redundant - input is default state)
  DDRD &= ~_BV(2);

  DDRB |= _BV(0);
  // Turn off PD4,5,6,7
  PORTD &= ~(_BV(4) | _BV(5) | _BV(6) | _BV(7));

  // Interrupt _BVs:

  // Set EICRA to falling on INT0
  EICRA = _BV(1);

  // Enable INT0 in EIMSK
  EIMSK = _BV(0);

  // ... make sure any interrupt is allowed at all
  // SREG = _BV(7);
  // Can also be written with assembler :D
  __asm("sei");

  // COUNTERS:
  // TCCR0A: timer/counter control register A
  // TCNT0: timer/counter register
  // OCR0A: output compare register A
  TCCR0A = 0;
  TCCR0B |= (_BV(1) | _BV(2));
}

ISR(INT0_vect) { increment(); }

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}