CCOPTS=-Wall -Wextra -Werror

.PHONY: all test clean pristine

all: testfs

image.o: image.c image.h
	gcc $(CCOPTS) -c image.c

block.o: block.c block.h
	gcc $(CCOPTS) -c block.c

libvvsfs.a: image.o block.o
	ar rcs $@ $^

testfs.o: testfs.c image.h block.h ctest.h
	gcc $(CCOPTS) -c testfs.c

testfs: testfs.o libvvsfs.a
	gcc $(CCOPTS) -DCTEST_ENABLE -o $@ testfs.o libvvsfs.a

test: testfs
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs libvvsfs.a test
