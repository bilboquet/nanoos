includes = $(find . -type f -name '*.h')
INCLUDE_PATH= -I ./include
CFLAGS = $(INCLUDE_PATH)
OBJS = main.o
CC = arm-none-eabi-gcc

all: nanoos.bin 

nanoos.bin: $(OBJS)
	$(CC) $(CLAGS) -o nanoos.bin $^

.PHONY: doc
doc:
	doxygen nanoos.doxygen

clean:
	rm -rf doc/
