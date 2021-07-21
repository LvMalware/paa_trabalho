all:
	gcc -o graphs main.c lib/*.c -Wall

clean:
	rm -rf graphs
