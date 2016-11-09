CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-g -lm

SOURCES=$(wildcard ./src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=./bin/evaluator

.PHONY: all bin clean


all: bin

bin: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS)
	-rm $(EXECUTABLE)
