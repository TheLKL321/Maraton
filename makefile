CC=gcc
CFLAGS= -Wall -Wextra -std=c11
DEPS = userTree.h

%.o: %.c $(DEPS)
	$(CC) -c $< $(CFLAGS)

main: main.o userTree.o 
	gcc -o main main.o userTree.o

clean:
	-rm main *.o