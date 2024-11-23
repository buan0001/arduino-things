## Traffic light, built and tested for Arduino Uno 3 (with atmega328p processor) on Windows 10.
## Required software
- Make (for the Makefile)
- Avr-gcc (to compile the C-code to the microcontroller)
- Avrdude (for uploading the code to the microcontroller)
## ... Other than that
- With Make installed, running `make all` should automatically run the program on a USB-connected arduino
- Do note the (usb)`port` used in the Makefile as well as the ports used for the seperate inputs/outputs - feel free to change these to your liking!
    - If using MAC or Linux you will most certainly have to change the ports at least