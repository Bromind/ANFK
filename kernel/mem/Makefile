CC = gcc
AS = /usr/bin/as
CFLAGS = -std=gnu99 -g -pg -Wall -DDEBUG
ASFLAGS = -g
RM = /bin/rm
OBJ = *.o
OUTPUT = kernel


main: allocationTable.o base.o freeMemory.o

base.o:
	$(AS) $(ASFLAGS) -o base.o base.s

clean:
	$(RM) $(OBJ)
