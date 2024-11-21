# Variables:
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200
PORT = COM3
PROGRAMMER = arduino

# Compiler and flags
CC = avr-gcc
# -Wall enables all compiler's warning messages
# -Os optimizes the code for size
# -DF_CPU defines the CPU frequency
# -mmcu specifies the target microcontroller
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
# AVRDUDE = C:\Users\Buster\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17
AVRDUDE = avrdude
# The different flags mean:
# -v verbose
# -p the part number
# -c the programmer type
# -P the port
# -b the baud rate
# -D disable auto erase for flash
# AVRDUDE_FLAGS = -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -D
AVRDUDE_FLAGS = -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D
FILE_NAME = counter0

all: compile hex avr_run


compile:
	$(CC) $(CFLAGS) -o ${FILE_NAME}.o ${FILE_NAME}.c

hex: ${FILE_NAME}.o
# -R .eeprom removes the eeprom section from the hex file
# -O ihex specifies the output format to be ihex
	avr-objcopy -O ihex ${FILE_NAME}.o ${FILE_NAME}.hex

avr_run:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:${FILE_NAME}.hex:i
#$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(file).hex:i

clean:
	rm *.o *.elf *.hex

clean_win:
	del *.o *.elf *.hex