CC=gcc
CFLAGS=$(shell pkg-config --cflags ncurses) -c -O3
LDFLAGS=$(shell pkg-config --libs ncurses) -o
SOURCES=game.c snake_body.c field.c
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
