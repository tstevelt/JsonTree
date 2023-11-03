
PROG = JsonTree
LIB = JsonTree.a
XLIB = -ljson-c
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

CFLAGS = -g -Wall -Wformat-truncation=0 -I/usr/local/include

FILES = \
	$(LIB)(JsonTree.o)		\
	$(LIB)(Functions.o)		\
	$(LIB)(ProcessArray.o)	\
	$(LIB)(ProcessObject.o)	\
	$(LIB)(LoadScript.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): JsonTree.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

