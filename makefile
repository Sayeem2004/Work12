all: dir.o
	gcc -o dir dir.o

dir.o: dir.c
	gcc -x c -c dir.c

run:
	./dir

clean:
	rm *.o
