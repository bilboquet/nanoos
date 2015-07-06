VPATH = include:src:lib:drv/bus/i2c::drv/bus/spi:drv/serial
INCLUDE_PATH = -I ./include -I ./lib -I ./drv/bus/i2c -I ./drv/bus/spi -I ./drv/serial
CFLAGS = $(INCLUDE_PATH)
OBJS = main.o device.o
TARGET = nanoos.bin
CC = arm-none-eabi-gcc


all: $(TARGET) 

nanoos.bin: $(OBJS)
	$(CC) $(CLAGS) -o $@ $^

main.o: main.c device.h

device.o: device.c device.h i2c.h uart.h

.PHONY: doc
doc:
	doxygen nanoos.doxygen

.PHONY: clean
clean:
	rm -rf doc/ $(TARGET)
