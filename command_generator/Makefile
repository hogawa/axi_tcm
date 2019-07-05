CC      = gcc
CFLAGS  = -std=c99 -Wall -O3 -g
DEFINES =
OBJS    =

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) -c $^ -o $(@F)

all: test

test: $(OBJS) test.o
	$(CC) -o test $(OBJS) test.o

clean:
	rm -rf *.o test