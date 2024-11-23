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
AVRDUDE_FLAGS = -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D

OBJCOPY = avr-objcopy 
# OBJCOPY_FLAGS = -O ihex a.out a.hex
OBJCOPY_FLAGS = -O ihex $(FILE_NAME).out $(FILE_NAME).hex

FILE_NAME = a
#FILE_NAME = traffic_light

all: compile hex avr_run



compile:
	$(CC) $(CFLAGS) -c *.c
	$(CC) $(CFLAGS) *.o

hex:
	$(OBJCOPY) $(OBJCOPY_FLAGS)

avr_run:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(FILE_NAME).hex:i

clean:
	rm *.o *.elf *.hex *.out

clean_win:
	del *.o *.elf *.hex *.out