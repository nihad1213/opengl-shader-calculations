# Compiler
CXX = g++
CC  = gcc

# Flags
CXXFLAGS = -std=c++23 -Wall -Wextra -I./include
CFLAGS   = -Wall -Wextra -I./include

# Libraries
LIBS = -lglfw -lGL -lm -ldl

# Directories
SRC_DIRS = src
BUILD_DIR = build
BIN_DIR   = bin

# Sources
CPP_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
C_SOURCES   = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Object files (mirror folder structure)
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES)) \
          $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Target
TARGET = $(BIN_DIR)/opengl_program

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/src

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile C++
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@make -s all
	@./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

rebuild: clean all

help:
	@echo "Targets:"
	@echo "  all     - Build"
	@echo "  run     - Build and run"
	@echo "  clean   - Remove build files"
	@echo "  rebuild - Clean then build"
	@echo "  help    - Show this help message"

.PHONY: all directories run clean rebuild help