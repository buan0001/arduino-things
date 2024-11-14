#define PORT_D *((volatile unsigned char*)0x2B)
#define PORT_D_DDR *((volatile unsigned char*)0x2A)
#define PORT_B *(volatile unsigned char*)0x25
#define PORT_B_DDR *(volatile unsigned char*)0x24
#define PORT_B_PIN *(volatile unsigned char*)0x23

#define NS_GREEN_CAR 1 << 0
#define NS_YELLOW_CAR 1 << 1
#define NS_RED_CAR 1 << 2
#define EW_GREEN_CAR 1 << 3
#define EW_YELLOW_CAR 1 << 4
#define EW_RED_CAR 1 << 5
#define EW_BUTTON_PRESSED 1 << 6
#define NS_BUTTON_PRESSED 1 << 7

#define NS_GREEN_PEDSTRIAN 1 << 0
#define NS_RED_PEDSTRIAN 1 << 1
#define EW_GREEN_PEDSTRIAN 1 << 2
#define EW_RED_PEDSTRIAN 1 << 3
#define NS_BUTTON_INPUT 1 << 4
#define EW_BUTTON_INPUT 1 << 5

char direction = 0;

void delay(volatile long time) {
  time *= 1000;
  while (time) {
    // if (direction == 1 && (PORT_D && ))
    time--;
  }
}

// Assumes NS_RED_CAR is already on
void ns_to_green() {
  // Turn on the yellow and wait
  // 100 | 010 == 111
  // 101 & 010 == 000
  PORT_D |= NS_YELLOW_CAR;
  delay(1000);
  // Turn off red and yellow
  // 1111
  // yellow: 010
  // red: 10000
  // red | yellow: 10010
  // NOT red | yellow: 10001
  PORT_D &= ~(NS_YELLOW_CAR | NS_RED_CAR);
  // Turn on green
  PORT_D |= NS_GREEN_CAR;
}

void ns_to_red() {
  PORT_D |= NS_YELLOW_CAR;
  delay(1000);
  PORT_D &= ~(NS_GREEN_CAR | NS_YELLOW_CAR);
  PORT_D |= NS_RED_CAR;
}

void ew_to_green() {
  // Turn on the yellow and wait
  PORT_D |= EW_YELLOW_CAR;
  delay(1000);
  // Turn off red and yellow
  PORT_D &= ~(EW_YELLOW_CAR | EW_RED_CAR);
  // Turn on green
  PORT_D |= EW_GREEN_CAR;
}

void ew_to_red() {
  PORT_D |= EW_YELLOW_CAR;
  delay(1000);
  PORT_D &= ~(EW_GREEN_CAR | EW_YELLOW_CAR);
  PORT_D |= EW_RED_CAR;
}

void ns_pedestrian_to_green() {
  PORT_B |= NS_GREEN_PEDSTRIAN;
  PORT_B &= ~(NS_RED_PEDSTRIAN);
}

void ns_pedestrian_to_red() {
  PORT_B |= NS_RED_PEDSTRIAN;
  PORT_B &= ~(NS_GREEN_PEDSTRIAN);
}
void ew_pedestrian_to_green() {
  PORT_B |= EW_GREEN_PEDSTRIAN;
  PORT_B &= ~(EW_RED_PEDSTRIAN);
}

void ew_pedestrian_to_red() {
  PORT_B |= EW_RED_PEDSTRIAN;
  PORT_B &= ~(EW_GREEN_PEDSTRIAN);
}

void interruptable_delay(volatile long time) {
  time *= 1000;
  // If time is bigger than min_time and the button is pressed in the correct
  // direction - delay with min_time instead
  long min_time = time * 0.2;

  while (time) {
    if (direction == 1 && NS_BUTTON_INPUT) {
      // Turn on the light indicating that the button has been pressed
      PORT_D |= NS_BUTTON_PRESSED;
      delay(min_time);
      break;
    }
  }
}

// TODO: Handle button press to delay time until next green.
int main() {
  // Change the data direction for the ports to input
  // The last 2 ports will be used to indicate whether the pedestrian crossing
  // "button" has been pressed or not The first 6 handle the lights for the cars
  PORT_D_DDR = 0b11111111;
  // Sets the lower 4 bits as output and the top 2 as input (for buttons)
  PORT_B_DDR |= 0b001111;
  // Start with both red
  PORT_D |= NS_RED_CAR | EW_RED_CAR;
  PORT_B |= NS_RED_PEDSTRIAN | EW_RED_PEDSTRIAN;

  delay(500);
  while (1) {
    // Handle north / south
    if (PORT_B_PIN & EW_BUTTON_INPUT | PORT_B_PIN & NS_BUTTON_INPUT) {
      PORT_D |= EW_BUTTON_PRESSED | NS_BUTTON_PRESSED;
    } else {
      PORT_D &= ~(EW_BUTTON_PRESSED | NS_BUTTON_PRESSED);
    }
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
    direction = 1;

    ew_to_green();
    delay(300);

    // Turn off pedestrian light if it was on
    PORT_D &= ~(EW_BUTTON_PRESSED);
    ew_pedestrian_to_green();

    delay(1500);

    ew_pedestrian_to_red();
    delay(500);

    if (PORT_B & EW_BUTTON_PRESSED) {
      PORT_D |= EW_BUTTON_PRESSED | NS_BUTTON_PRESSED;
    } else {
      PORT_D &= ~(EW_BUTTON_PRESSED | NS_BUTTON_PRESSED);
    }

    ew_to_red();

    direction = 0;
  }

  return 0;
}
