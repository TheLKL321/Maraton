CC=gcc
CFLAGS= -Wall -Wextra -std=c11 -g

%.o: %.c 
	$(CC) -c $< $(CFLAGS)

main: main.o movieTree.o
	gcc -o main main.o movieTree.o

clean:
	-rm main *.o