# Michael Bartling

# Compiler Information
CC = gcc

# Compiler Flags
#CFLAGS = -c -Wall
CFLAGS = -Wall -o
# Linker Flags
LDFLAGS = 

# Sources

EXECUTABLES = reduce.out map.out

OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLES)

reduce.out:
	$(CC) $(CFLAGS) reduce.out reduce_words.c
map.out:
	$(CC) $(CFLAGS) map.out map_words.c

#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
#.c.o:
#	$(CC) $(CFLAGS) $< -o $@
# clean
clean: 
	rm -rf *.o $(EXECUTABLES)
