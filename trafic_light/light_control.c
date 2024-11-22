#include "light_control.h"
#include "ports_and_bits.h"

void delay(char seconds) {
  time *= 1000;
  while (time) {
    time--;
  }
}

void ns_to_green() {
  // Turn on the yellow and wait
  // 100 | 010 == 111
  // 101 & 010 == 000
  PORT_B |= NS_YELLOW_CAR;
  delay(1000);
  // Turn off red and yellow
  // 1111
  // yellow: 010
  // red: 10000
  // red | yellow: 10010
  // NOT red | yellow: 10001
  PORT_B &= ~(NS_YELLOW_CAR | NS_RED_CAR);
  // Turn on green
  PORT_B |= NS_GREEN_CAR;
}

void ns_to_red() {
  PORT_B |= NS_YELLOW_CAR;
  delay(1000);
  PORT_B &= ~(NS_GREEN_CAR | NS_YELLOW_CAR);
  PORT_B |= NS_RED_CAR;
}

void ew_to_green() {
  // Turn on the yellow and wait
  PORT_B |= EW_YELLOW_CAR;
  delay(1000);
  // Turn off red and yellow
  PORT_B &= ~(EW_YELLOW_CAR | EW_RED_CAR);
  // Turn on green
  PORT_B |= EW_GREEN_CAR;
}

void ew_to_red() {
  PORT_B |= EW_YELLOW_CAR;
  delay(1000);
  PORT_B &= ~(EW_GREEN_CAR | EW_YELLOW_CAR);
  PORT_B |= EW_RED_CAR;
}

void ns_pedestrian_to_green() {
  PORT_D |= NS_GREEN_PEDSTRIAN;
  // Turn off red as well as the button pressed
  PORT_D &= ~(NS_RED_PEDSTRIAN | NS_BUTTON_PRESSED);
}

void ns_pedestrian_to_red() {
  PORT_D |= NS_RED_PEDSTRIAN;
  PORT_D &= ~(NS_GREEN_PEDSTRIAN);
}
void ew_pedestrian_to_green() {
  PORT_D |= EW_GREEN_PEDSTRIAN;
  // Turn off red as well as the button pressed
  PORT_D &= ~(EW_RED_PEDSTRIAN | EW_BUTTON_PRESSED);
}

void ew_pedestrian_to_red() {
  PORT_D |= EW_RED_PEDSTRIAN;
  PORT_D &= ~(EW_GREEN_PEDSTRIAN);
}