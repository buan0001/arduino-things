#include <stdio.h>
#define NS_GREEN_BIT 1 << 0
#define NS_YELLOW_BIT 1 << 1
#define NS_RED_BIT 1 << 2
#define EW_GREEN_BIT 1 << 3
#define EW_YELLOW_BIT 1 << 4
#define EW_RED_BIT 1 << 5

int main() {
    // RED: 100 = 4
    // YELLOW: 10 = 2
    // RED + YELLOW: 110 = 6
  int light = NS_RED_BIT;
  light |= NS_YELLOW_BIT;
  light &= ~( NS_YELLOW_BIT | NS_RED_BIT);
  printf("yellow + red: %d\n", 4 + 6);
//   light |= NS_GREEN_BIT;
  printf("light: %d\n", light);
  return 0;
}