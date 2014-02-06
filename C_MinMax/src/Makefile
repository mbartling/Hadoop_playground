# Michael Bartling

# Compiler Information
CC = gcc

# Compiler Flags
#CFLAGS = -c -Wall
CFLAGS = -Wall -o
# Linker Flags
LDFLAGS = 

# Sources

EXECUTABLES = myCollect geninputs myReduce myMap mySimpleCollect mySimpleMap

OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLES)

myCollect:
	$(CC) $(CFLAGS) myCollect collect.c
geninputs:
	$(CC) $(CFLAGS) geninputs geninputs.c
myReduce:
	$(CC) $(CFLAGS) myReduce reduce.c
myMap:
	$(CC) $(CFLAGS) myMap map.c

mySimpleCollect:
	$(CC) $(CFLAGS) mySimpleCollect collect_simple.c
mySimpleMap:
	$(CC) $(CFLAGS) mySimpleMap map_simple.c	
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
#.c.o:
#	$(CC) $(CFLAGS) $< -o $@
# clean
clean: 
	rm -rf *.o $(EXECUTABLES)