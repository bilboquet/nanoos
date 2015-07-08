VPATH = include:src:lib:drv/bus/i2c::drv/bus/spi:drv/serial
INCLUDE_PATH = -I ./include -I ./lib -I ./drv/bus/i2c -I ./drv/bus/spi -I ./drv/serial
CFLAGS = $(INCLUDE_PATH)
OBJS = main.o device.o uart.o
TARGET = nanoos.bin
CC = arm-none-eabi-gcc


all: $(TARGET) 

nanoos.bin: $(OBJS)
	$(CC) $(CLAGS) -o $@ $^

main.o: main.c device.h

device.o: device.c device.h i2c.h uart.h

uart.o: uart.c uart.h device.h


.PHONY: doc clean
doc:
	doxygen nanoos.doxygen

flow:
	cflow main.c lib/time.c src/device.c drv/serial/uart.c

clean:
	rm -rf doc/ $(TARGET)
