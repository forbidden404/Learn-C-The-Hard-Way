CFLAGS=-Wall -g -Wno-unused-function

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

all:
	$(CC) $(CFLAGS) -DNDEBUG -c $(SOURCES)
	$(CC) $(CFLAGS) -DNDEBUG -o logfind $(OBJECTS)

clean:
	rm -f logfind
	rm -rf logfind.dSYM/
	rm -f *.o
