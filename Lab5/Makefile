CFLAGS = -Wall -g
Input = 4.c
Output = test

all: build run

build: $(Input)
	gcc -Wall -o $(Output) $(Input)

run:
	./$(Output)

.PHONY: clean

clean:
	rm -f *.o *~ $(Output)
