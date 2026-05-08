.PHONY: all clean pristine

all: main

main.o: main.c adder.h
	gcc -Wall -Wextra -c main.c

adder.o: adder.c
	gcc -Wall -Wextra -c adder.c

main: main.o adder.o
	gcc -Wall -Wextra -o main main.o adder.o

clean:
	rm -f *.o

pristine: clean
	rm -f main