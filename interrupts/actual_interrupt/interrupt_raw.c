#define port(adr) (*((volatile unsigned char*)adr))

// PORTB 0-7 (0-5 available)
#define PINB port(0x23)
#define DDRB port(0x24)
#define PORTB port(0x25)

// PORTC 0-6 (0-5 available)
#define PINC port(0x26)
#define DDRC port(0x27)
#define PORTC port(0x28)

// PORTD 0-7 (0+1 for serial, but available)
#define PIND port(0x29)
#define DDRD port(0x2a)
#define PORTD port(0x2b)

// jeg har en ekstra lille hjælpermacro til bits
#define bit(n) (1 << n)

// Toggles what kind of "events" will trigger an interrupt
// First 4 bits are usable and are as follows:
// 11 10 01 00
// 00 - low level triggers interrupt
// 01 - both falling and rising generate interrupt
// 02 - only falling generates interrupt
// 03 - only rising generates interrupt
#define EICRA port(0x69)

// Interrupt "mask"
// Determine whether interrupts are open at all on specific interrupt ports
// Only 2 first bits are useable:
// Bit 0: Toggles INT0
// Bit 1: Toggles INT1
#define EIMSK port(0x3d)

// Is an "interrupt flag"
// The interrupts defined in EICRA will only cause an interrupt IFF:
// The corresponding EIMSK bit is set AND
// The EIFR bit is cleared

// The corresponding EIFR bit will always be set after an interrupt is caused
// So it must be cleared before another interrupt on the same bit can be caused
// ... setting the __attribute__(interrupt) automatically clears the flag
// afterwards, however. So this is unnecessary here :d
#define EIFR port(0x3c)

// AVR Status Register
// Bit 7 MUST be set for any interrupt to be enabled
#define SREG port(0x5f)

void increment() { PORTB ^= bit(0); }

void wait() {
  for (volatile long i = 0; i < 1000000; i++) {
  }
}

void loop() {
  static unsigned char count = 0;
  // turn of upper bits - then set them again with the counter
  PORTD = (PORTD & 0x0f) | (count << 4);
  // increment counter, completely forget about the upper 4 bits
  count++;

  wait();
}

void setup() {
  // Set PD4,5,6,7 as output
  DDRD |= (bit(4) | bit(5) | bit(6) | bit(7));

  // Set PD2 as input (kind of redundant - input is default state)
  DDRD &= ~bit(2);

  DDRB |= bit(0);
  // Turn off PD4,5,6,7
  PORTD &= ~(bit(4) | bit(5) | bit(6) | bit(7));

  // Interrupt bits:

  // Set EICRA to falling on INT0
  EICRA = bit(1);

  // Enable INT0 in EIMSK
  EIMSK = bit(0);

  // ... make sure any interrupt is allowed at all
  // SREG = bit(7);
  // Can also be written with assembler :D
  __asm("sei");
}

void __vector_1(void) __attribute__((interrupt));

void __vector_1(void) { increment(); }

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}