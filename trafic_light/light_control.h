#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H
void delay(volatile long time);
void ns_to_green();
void ns_to_red();
void ew_to_green();
void ew_to_red();
void ns_pedestrian_to_green();
void ns_pedestrian_to_red();
void ew_pedestrian_to_green();
void ew_pedestrian_to_red();

#define CLOCK_SPEED 16000000
#define TICKS_PER_SECOND (CLOCK_SPEED / 1024)

#endif