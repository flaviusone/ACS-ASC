#
# Author: Flavius Costin Tirnacop 331CA
# Tema 2 ASC
#
CC=gcc
CFLAGS=-g -Wall
OBJ=main.o
TARGET=main
MAT_SIZE=10



all: clean build run

run:
	./main $(MAT_SIZE)

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

clean:
	rm -rf $(OBJ) $(TARGET) *~
