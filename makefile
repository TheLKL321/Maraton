CC=gcc
CFLAGS= -Wall -Wextra -std=c11 -O2

%.o: %.c 
	$(CC) -c $< $(CFLAGS)

main: main.o movieTree.o
	gcc -o main main.o movieTree.o

clean:
	-rm main *.o