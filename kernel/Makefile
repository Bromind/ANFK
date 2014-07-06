CC = gcc
AS = /usr/bin/as
CFLAGS = -std=c99 -g -Wall
ASFLAGS = -g
RM = /bin/rm
OBJ = *.o


main: mainTest

cleanMain: clean main

mainTest: transfer.o processManager.o processTest.o
	$(CC) -o test $(LDFLAGS) $^ $(LDLIBS)

transfer.o:
	$(AS) $(ASFLAGS) -o transfer.o transfer.s

clean:
	$(RM) $(OBJ)
