CC=gcc
CFLAGS=-c -Wall -O3 -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
SOURCES=main.c utils.c

OBJECTS=$(SOURCES:.c=.o)
EXECS=act05
RM=/bin/rm

.PHONY: all
all: $(SOURCES) $(EXECS)
	
$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	$(CC) $(CFLAGS) player.c -o player.o
	$(CC) utils.o player.o -o player
	$(CC) alarm.c -o alarm
	$(RM) *.o

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) -rf *.o
	$(RM) -rf $(EXECS)
	$(RM) -rf player 
	$(RM) -rf alarm

.PHONY: run
run: $(EXECS)
	 ./$(EXECS)