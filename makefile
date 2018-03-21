CC=gcc
CFLAGS= -Wall -Wextra -std=c11
DEPS = userTree.h

%.o: %.c $(DEPS)
	$(CC) -c $< $(CFLAGS)

main: main.o movieTree.o 
	gcc -o main main.o movieTree.o

clean:
	-rm main *.o