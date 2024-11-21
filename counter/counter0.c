#include <avr/interrupt.h>
#include <avr/io.h>

void increment() { TCNT0++; }

void wait() {
  for (volatile long i = 0; i < 1000000; i++) {
  }
}

void loop() { PORTB = TCNT0 & 0x0f; }

void setup() {
  PORTB |= _BV(0) | _BV(1) | _BV(2) | _BV(3);

  // OC0A is on PD6 - so we just set the data direction on DDRD
  DDRD |= (_BV(PD6) | _BV(PD5));
  PORTD = _BV(5);
  // PAGE 113 IN THE DOCS
  // TCCR0A: timer/counter control register A
  // TCNT0: timer/counter register
  // OCR0A: output compare register A
  // COM0A0: Toggle OC0A on Compare Match
  // WGM01: CTC (Clear Timer on Compare Match) mode
  // AKA port 0x24(0x44)
  TCCR0A = _BV(WGM01) | _BV(COM0A0);
  // TCCR0B: Timer/Counter Control Register B
  // Set count source to T0 falling
  TCCR0B |= (_BV(CS01) | _BV(CS02));
  OCR0A = 3;
  // Enable interrupts from compare match A
  TIMSK0 = _BV(OCIE0A);
  // Enable external interrupt
  sei();
  // An alternative is to write to SREG (0x5f) directly
  // SREG |= _BV(SREG_I) (the I bit is the 7th bit);
}

ISR(INT0_vect) { increment(); }

ISR(TIMER0_COMPA_vect) {
  PORTD ^= _BV(5);
  if (OCR0A == 3) {
    OCR0A = 10;
  } else {
    OCR0A = 3;
  }
}

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}