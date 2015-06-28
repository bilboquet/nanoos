includes = $(find . -type f -name '*.h')
objs = main.o
CC = arm-none-eabi-gcc

all: nanoos.bin 

nanoos.bin: $(objs)
	$(CC) -o nanoos.bin $^

.PHONY: doc
doc:
	doxygen nanoos.doxygen

clean:
	rm -rf doc/
