all: main
main: main.o FileA.o
	g++ -o main main.o FileA.o
main.o:
	g++ -c main.c
FileA.o:
	g++ -c FileA.c
clean:
	rm -rf main.o main FileA.o
