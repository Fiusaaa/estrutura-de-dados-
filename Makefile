all: main

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -o main.o main.c -c

run:
	./main
