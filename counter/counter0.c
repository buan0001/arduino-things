#include <avr/io.h>
// #include <avr/iom328p.h>
#include <avr/interrupt.h>

void toggle() {
  PORTB ^= _BV(0);
  TCNT0++;
}

void wait() {
  for (volatile long i = 0; i < 1000000; i++) {
  }
}

void loop() { PORTB = TCNT0 & 0x0f; }

void setup() {
  // COUNTERS:
  // TCCR0A: timer/counter control register A
  // TCNT0: timer/counter register
  // OCR0A: output compare register A
  TCCR0A = _BV(WGM01);
  // Set count source to T0 falling
  TCCR0B |= (_BV(CS01) | _BV(CS02));
  OCR0A = 9;
}

ISR(INT0_vect) { toggle(); }

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}