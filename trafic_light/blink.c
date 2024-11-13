#define PORT_D *((volatile unsigned char*)0x2B)
#define PORT_D_DDR *((volatile unsigned char*)0x2A)

#define NS_GREEN_BIT 1 << 0
#define NS_YELLOW_BIT 1 << 1
#define NS_RED_BIT 1 << 2
#define EW_GREEN_BIT 1 << 3
#define EW_YELLOW_BIT 1 << 4
#define EW_RED_BIT 1 << 5

void delay(volatile long time) {
  time *= 1000;
  while (time) {
    time--;
  }
}

// Assumes NS_RED_BIT is already on
void ns_to_green() {
  // Turn on the yellow and wait
  PORT_D |= NS_YELLOW_BIT;
  delay(1000);
  // Turn off red and yellow
  PORT_D &= ~(NS_YELLOW_BIT | NS_RED_BIT);
  // Turn on green
  PORT_D |= NS_GREEN_BIT;
}

void ns_to_red() {
  PORT_D |= NS_YELLOW_BIT;
  delay(1000);
  PORT_D &= ~(NS_GREEN_BIT | NS_YELLOW_BIT);
  PORT_D |= NS_RED_BIT;
}

void ew_to_green() {
  // Turn on the yellow and wait
  PORT_D |= EW_YELLOW_BIT;
  delay(1000);
  // Turn off red and yellow
  PORT_D &= ~(EW_YELLOW_BIT | EW_RED_BIT);
  // Turn on green
  PORT_D |= EW_GREEN_BIT;
}

void ew_to_red() {
  PORT_D |= EW_YELLOW_BIT;
  delay(1000);
  PORT_D &= ~(EW_GREEN_BIT | EW_YELLOW_BIT);
  PORT_D |= EW_RED_BIT;
}


int main() {
  // Change the data direction for ports to input
  PORT_D_DDR = 0b11111111;
  // Start with both red
  PORT_D |= NS_RED_BIT | EW_RED_BIT;
  delay(500);
  while (1) {
    // Handle north / south
    ns_to_green();
    delay(2500);
    ns_to_red();

    delay(250);

    // Change direction
    ew_to_green();
    delay(2500);
    ew_to_red();
  }

  return 0;
}
