# Michael Bartling

# Compiler Information
CC = gcc

# Compiler Flags
#CFLAGS = -c -Wall
CFLAGS = -Wall -o
# Linker Flags
LDFLAGS = 

# Sources
#SOURCES = geninputs.c
#EXECUTABLE = geninputs
#SOURCES = reduce.c
#EXECUTABLE = myReduce
#SOURCES = map.c
#EXECUTABLE = myMap
#SOURCES = collect.c
#EXECUTABLE = myCollect
EXECUTABLES = myCollect geninputs myReduce myMap

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
	
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
#.c.o:
#	$(CC) $(CFLAGS) $< -o $@
# clean
clean: 
	rm -rf *.o $(EXECUTABLES)