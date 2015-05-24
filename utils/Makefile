CC = gcc
CFLAGS = -std=c99 -Wall# -g -pg
RM = /bin/rm
OBJ = *.o
OUTPUT = utilsTest

all: array.o linkedList.o

test: array.o linkedList.o mainTest.o
	$(CC) -o $(OUTPUT) $(LDFLAGS) $^ $(LDLIBS)

clean:
	$(RM) $(OBJ)
	$(RM) $(OUTPUT)
