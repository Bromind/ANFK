CC = gcc
AS = /usr/bin/as
CFLAGS = -std=c99 -g -pg -Wall -DDEBUG
ASFLAGS = -g
RM = /bin/rm
OBJ = *.o
OUTPUT = kernel


main: allocationTable.o freeSpace.o

freeSpace.o:
	$(AS) $(ASFLAGS) -o freeMemory.o freeMemory.s

clean:
	$(RM) $(OBJ)
