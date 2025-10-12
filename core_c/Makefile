CC = gcc
CFLAGS = -I. -shared -fPIC -Wall -o pim.so

all: pim.so

pim.so: structs.h login.c data.c main.c
	$(CC) $(CFLAGS) login.c data.c main.c

test:
	$(CC) -o test main.c login.c data.c
	./test

clean:
	rm -f *.so test *.o
