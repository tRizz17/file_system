.PHONY: all test clean pristine

all: testfs

image.o: image.c image.h
	gcc -Wall -Wextra -Werror -c image.c

block.o: block.c block.h
	gcc -Wall -Wextra -Werror -c block.c

libvvsfs.a: image.o block.o
	ar rcs $@ $^

testfs: image.o block.o
	gcc -Wall -Wextra -Werror -o main image.o block.o

test:
	make
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs
