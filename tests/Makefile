# compiler
CC = gcc
# flags
CFLAGS = -g -Wall -D_DEFAULT_SOURCE -std=c11 -Werror

# name objects
OBJ = main.o hashtable.o tests.o


all: hashtable end

main.o: main.c 
	$(CC) $(CFLAGS) -c -o main.o main.c 

hashtable.o: ../hashtable.c 
	$(CC) $(CFLAGS) -c -o hashtable.o ../hashtable.c 

tests.o: tests.c
	$(CC) $(CFLAGS) -c -o tests.o tests.c

hashtable: $(OBJ)
	$(CC) -o tests main.o hashtable.o tests.o -lm

clean: 
	rm -rf hashtable *.o

end:
	rm -f tests.o
	rm -f hashtable.o
	rm -f main.o