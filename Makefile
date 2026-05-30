CCOPTS=-Wall -Wextra -Werror

.PHONY: all test clean pristine

all: testfs

image.o: image.c image.h
	gcc $(CCOPTS) -c image.c

block.o: block.c block.h image.h free.h
	gcc $(CCOPTS) -c block.c

free.o: free.c free.h block.h
	gcc $(CCOPTS) -c free.c

inode.o: inode.c inode.h free.h block.h pack.h
	gcc $(CCOPTS) -c inode.c

pack.o: pack.c pack.h
	gcc $(CCOPTS) -c pack.c

mkfs.o: mkfs.c image.h block.h inode.h pack.h
	gcc $(CCOPTS) -c mkfs.c

libvvsfs.a: image.o block.o free.o inode.o pack.o mkfs.o
	ar rcs $@ $^

testfs.o: testfs.c image.h block.h free.h ctest.h inode.h pack.h mkfs.h
	gcc $(CCOPTS) -c testfs.c

testfs: testfs.o libvvsfs.a
	gcc $(CCOPTS) -DCTEST_ENABLE -o $@ testfs.o libvvsfs.a

test: testfs
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs libvvsfs.a test ialloc_test alloc_test file_system
