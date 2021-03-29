CC=gcc
CFLAGS=$(shell pkg-config --cflags ncurses) -c
LDFLAGS=$(shell pkg-config --libs ncurses) -o
SOURCES=hello.c snake_body.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE=snake

.PHONY: all clean

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) $@

%.o: %.c
	$(CC) $(CFLAGS) $<
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
