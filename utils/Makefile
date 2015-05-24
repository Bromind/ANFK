CC = gcc
CFLAGS = -std=c99 -Wall# -g -pg
RM = /bin/rm
OBJ = $(patsubst %c,%o,$(wildcard *.c))

SUBDIR = string maths

all: $(OBJ) $(SUBDIR)

$(SUBDIR): force_look
	$(MAKE) -C $@

force_look:
	true

clean:
	-$(RM) $(OBJ)
	-for d in $(SUBDIR); do (cd $$d ; $(MAKE) clean); done
