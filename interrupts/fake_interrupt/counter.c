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

void toggle() { PORTB ^= bit(0); }

void wait() {
  
  for (volatile long i = 0; i < 1000000; i++) {
    
    if (~PIND & bit(2) && i < 800000) {
      i = 800000;
      toggle();
    }
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

  DDRD &= ~bit(2);

  DDRB |= bit(0);
  // Turn of PD4,5,6,7
  PORTD &= (bit(4) | bit(5) | bit(6) | bit(7));
}

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}