CC = gcc
CFLAGS = -Wall -g
OBJS = malloc.o test_malloc.o

all: test

malloc.o: malloc.c malloc.h
	$(CC) $(CFLAGS) -c malloc.c

test_malloc.o: test_malloc.c malloc.h
	$(CC) $(CFLAGS) -c test_malloc.c

test: $(OBJS)
	$(CC) $(CFLAGS) -o test $(OBJS)

clean:
	rm -f *.o test
