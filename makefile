# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g  # Enable warnings and debugging
LDFLAGS =  # Add linker flags if needed

# Source and Object Files
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)  # Finds all .c files in src/
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))  # Converts .c to .o
BIN = $(BIN_DIR)/compiler  # Output executable

# Default Target
all: $(BIN)

# Create binary executable
$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
