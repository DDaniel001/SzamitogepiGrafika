APP_NAME = blacksmith

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32 -lobj -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

TARGET = $(APP_NAME).exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /q src\*.o $(TARGET) 2>nul