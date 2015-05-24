
CC = gcc
CFLAGS = -std=c99 -Wall# -g -pg
RM = /bin/rm
OBJ = $(patsubst %c,%o,$(wildcard *.c))

all: $(OBJ)

clean:
	-$(RM) $(OBJ)
