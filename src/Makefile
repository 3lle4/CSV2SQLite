CC = gcc
CFLAGS = -Wall

# List all your source files here
SOURCES = main.c readCSV.c

# Derive object file names from source file names
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = CSV2SQLite

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
