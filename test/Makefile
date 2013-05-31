#compiler and flags
CC=gcc
CFLAGS=-c -Wall -W
LIBS=-lcheck

#objects, libraries, and main
OBJ=test_serial.o serial.o
LIB=../serial/serial.c
MAIN=test_serial.c


OUTPUT=testserial #executable output

all: test_serial

test_serial: $(OBJ)
	$(CC) $(OBJ) -o $(OUTPUT) $(LIBS)
test_serial.o: $(MAIN)
	$(CC) $(CFLAGS) $(MAIN)
serial.o: $(LIB)
	$(CC) $(CFLAGS) $(LIB)

clean:
	rm -rf *.o $(OUTPUT)
