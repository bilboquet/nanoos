all: doc

doc:
	doxygen nanoos

clean:
	rm -rf doc/
