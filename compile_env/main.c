#define F_CPU 16000000UL  // Define CPU frequency as 16 MHz
#define MY_PORT_B_DDR *((volatile unsigned char*)0x24)
#define MY_PORT_B *((volatile unsigned char*)0x25)

// void delay_ms(long ms) {
//   volatile unsigned long delay_count = ms * 100000;
//   for (volatile long i = 0; i < delay_count; i++){}
//   // while (delay_count--) {
//   //   __asm__ __volatile__("nop");
//   // }
// }

void delay_ms(unsigned int ms) {
  while (ms--) {
    unsigned long delay_count = F_CPU / 8000;
    while (delay_count--) {
      __asm__ __volatile__ ("nop");
    }
  }
}

int main() {
  MY_PORT_B_DDR |= (1 << 5);  // Set PB5 as output

  while (1) {
    MY_PORT_B |= (1 << 5);  // Set PB5 high
    delay_ms(1500);
    MY_PORT_B &= ~(1 << 5);  // Set PB5 low
    delay_ms(500);
  }

  return 0;
}