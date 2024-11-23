#ifndef PORT_BIT_H
#define PORT_BIT_H
#define PORT_D *((volatile unsigned char*)0x2B)
#define PORT_D_DDR *((volatile unsigned char*)0x2A)
#define PORT_B *(volatile unsigned char*)0x25
#define PORT_B_DDR *(volatile unsigned char*)0x24
#define PORT_C_DDR *(volatile unsigned char*)0x28
#define PORT_B_PIN *(volatile unsigned char*)0x23

#define NS_GREEN_CAR 1 << 0
#define NS_YELLOW_CAR 1 << 1
#define NS_RED_CAR 1 << 2
#define EW_GREEN_CAR 1 << 3
#define EW_YELLOW_CAR 1 << 4
#define EW_RED_CAR 1 << 5

#define EW_BUTTON_PRESSED 1 << 0
#define NS_BUTTON_PRESSED 1 << 1
#define EW_GREEN_PEDSTRIAN 1 << 4
#define EW_RED_PEDSTRIAN 1 << 5
#define NS_GREEN_PEDSTRIAN 1 << 6
#define NS_RED_PEDSTRIAN 1 << 7

#define INTERRUPT0 (1 << 2)
#define INTERRUPT1 (1 << 3)

#define bit(num) (1 << num)

#endif