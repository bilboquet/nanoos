VPATH = include
INCLUDE_PATH = -I ./include
CFLAGS = $(INCLUDE_PATH)
OBJS = main.o
TARGET = nanoos.bin
CC = arm-none-eabi-gcc

all: $(TARGET) 

nanoos.bin: $(OBJS)
	$(CC) $(CLAGS) -o $@ $^

.PHONY: doc
doc:
	doxygen nanoos.doxygen

clean:
	rm -rf doc/ $(TARGET)
