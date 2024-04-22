all: target execute clean

execute:
	./main

target: main.o
	gcc main.o -o main

main.o:
	gcc -std=c99 -Wall -c main.c -o main.o

clean:
	rm -rf *.o main
