includes = $(find . -type f -name '*.h')

all: doc

.PHONY: doc
doc:
	doxygen nanoos.doxygen

clean:
	rm -rf doc/
