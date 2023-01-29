CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=gettimes.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gettimes

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
