CC=g++
#CFLAGS=-Wall
CFLAGS=
SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Compiler

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm $(EXECUTABLE)
	rm -f *.o *~ *.so

