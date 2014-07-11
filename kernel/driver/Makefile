AS = /usr/bin/as
ASFLAGS = -g
CC = gcc
CFLAGS = -std=c99 -g -pg -Wall
RM = /bin/rm
OBJ = *.o

all: drawing.o frameBuffer.o gpio.o mailbox.o systemTimer.o

mainTest: drawing.o frameBuffer.o gpio.o mailbox.o systemTimer.o mainTest.o
	$(CC) -o test $(CFLAGS) $^

clean: 
	$(RM) $(OBJ)
