VPATH = include:src
INCLUDE_PATH = -I ./include
CFLAGS = $(INCLUDE_PATH)
OBJS = main.o device.o
TARGET = nanoos.bin
CC = arm-none-eabi-gcc


all: $(TARGET) 

nanoos.bin: $(OBJS)
	$(CC) $(CLAGS) -o $@ $^

main.o: main.c device.h

device.o: device.c device.h i2c.h usart.h

.PHONY: doc
doc:
	doxygen nanoos.doxygen

.PHONY: clean
clean:
	rm -rf doc/ $(TARGET)
