#include "light_control.h"
#include <avr/io.h>
#include "ports_and_bits.h"


volatile char nextStageReady = 0; // Is used internally in the transition functions
void set_next_stage(char stage) { nextStageReady = stage; }

void ns_to_green() {
  PORTB |= NS_YELLOW_CAR;
  set_timer(1);
  while (1) {
    if (nextStageReady) {
      PORTB &= ~(NS_YELLOW_CAR | NS_RED_CAR);
      // Turn on green
      PORTB |= NS_GREEN_CAR;
      break;
    }
  }
}

void ns_to_red() {
  PORT_B |= NS_YELLOW_CAR;
  set_timer(1);
  while (1) {
    if (nextStageReady) {
      PORT_B &= ~(NS_GREEN_CAR | NS_YELLOW_CAR);
      PORT_B |= NS_RED_CAR;
      break;
    }
  }
}

void ew_to_green() {
  // Turn on the yellow and wait
  PORT_B |= EW_YELLOW_CAR;
  set_timer(1);
  while (!nextStageReady) {
    // Wait for the timer to finish
  }
  // Turn off red and yellow
  PORT_B &= ~(EW_YELLOW_CAR | EW_RED_CAR);
  // Turn on green
  PORT_B |= EW_GREEN_CAR;
}

void ew_to_red() {
  PORT_B |= EW_YELLOW_CAR;
  set_timer(1);
  while (!nextStageReady) {
    // Wait for the timer to finish
  }
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