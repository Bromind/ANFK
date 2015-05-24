CC = gcc
AS = /usr/bin/as
CFLAGS = -std=c99 -Wall #-g -pg #-DDEBUG
ASFLAGS = #-g
RM = /bin/rm
OBJ = *.o
OUTPUT = kernel


main: transfer.o processManager.o ../../utils/linkedList.o

transfer.o:
	$(AS) $(ASFLAGS) -o transfer.o transfer.s

clean:
	$(RM) $(OBJ)
