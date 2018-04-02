# compiler
CC = gcc-7
# flags
CFLAGS = -g -Wall -D_BSD_SOURCE -std=c11 -Werror

# name objects
OBJ = main.o hashtable.o example.o

.PHONY: all clean hashtable

all: hashtable

main.o: main.c 
	$(CC) $(CFLAGS) -c -o main.o main.c 

hashtable.o: hashtable.c 
	$(CC) $(CFLAGS) -c -o hashtable.o hashtable.c 

example.o: example.c
	$(CC) $(CFLAGS) -c -o example.o example.c

hashtable: $(OBJ)
	$(CC) -o hashtable main.o hashtable.o example.o -lm

clean: 
	rm -rf hashtable *.o

