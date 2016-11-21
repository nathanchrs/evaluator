CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-lm
LATEX=pdflatex

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

doc:
	-$(LATEX) doc/report.tex -output-directory doc

clean:
	-rm $(OBJECTS)
	-rm $(EXECUTABLE)
	-rm doc/report.aux
	-rm doc/report.log
	-rm doc/report.pdf
