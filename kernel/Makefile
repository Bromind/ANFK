CC = gcc
AS = /usr/bin/as
CFLAGS = -std=c99 -g -pg -Wall -DDEBUG
ASFLAGS = -g
RM = /bin/rm
OBJ = *.o
OUTPUT = kernel


main: mainTest

cleanMain: clean main

mainTest: transfer.o processManager.o processTest.o ../utils/linkedList.o
	$(CC) $(CFLAGS) -o $(OUTPUT) $(LDFLAGS) $^ $(LDLIBS)

transfer.o:
	$(AS) $(ASFLAGS) -o transfer.o transfer.s

clean:
	$(RM) $(OBJ)
	$(RM) $(OUTPUT)
