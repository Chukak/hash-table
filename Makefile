# compiler
CC = gcc-7
# flags
CFLAGS = -g -Wall -std=c11 

# name objects
OBJ = main.o hashtable.o
# headers
HEADERS = hashtable.h

.PHONY: all clean hashtable

all: hashtable

hashtable: $(OBJ)
	$(CC) -o hashtable main.o hashtable.o -lm

main.o: main.c 
	$(CC) $(CFLAGS) -c -o main.o main.c 

hashtable.o: hashtable.c 
	$(CC) $(CFLAGS) -c -o hashtable.o hashtable.c 

clean: 
	rm -rf hashtable *.o




