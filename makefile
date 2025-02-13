# Compiler and Flags

CC = gcc
CFLAGS = -Iinclude 
LDFLAGS =  # Add linker flags if needed

# Source and Object Files
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test/lexer_tc

SRC = $(wildcard $(SRC_DIR)/*.c)  # Finds all .c files in src/
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))  # Converts .c to .o
BIN = $(BIN_DIR)/compiler  # Output executable

TESTFILE = $(TEST_DIR)/t1.txt

# Default Target
all: $(BIN)
	$(BIN) $(TESTFILE)

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
