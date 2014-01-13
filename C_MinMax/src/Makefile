# Michael Bartling

# Compiler Information
CC = gcc

# Compiler Flags
CFLAGS = -c -Wall

# Linker Flags
LDFLAGS = 

# Sources
SOURCES = geninputs.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = geninputs

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@
# clean
clean: rm -rf *o 