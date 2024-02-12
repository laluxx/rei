PROJECT_NAME := rei 
CC := gcc
INCLUDE_DIRS := -I/usr/local/include
LIBRARY_PATHS := -L/usr/local/lib
LIBRARIES := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CFLAGS := -Wall -std=c99 $(INCLUDE_DIRS)
SOURCES := ./*.c
OBJECTS := $(SOURCES:.c=.o)
OUTPUT := $(PROJECT_NAME)

all: $(OUTPUT)
$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(OUTPUT) $(LIBRARY_PATHS) $(LIBRARIES)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -f $(OBJECTS) $(OUTPUT)
.PHONY: all clean
