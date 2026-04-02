# Compiler and Linker
CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -Isrc 
CFLAGS   := -O3
LDFLAGS  := 

# Directories
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

# Name of final program
TARGET    := $(BIN_DIR)/simulation

# Find all C/C++ files for logic
CPP_SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
C_SOURCES   := $(shell find $(SRC_DIR) -name "*.c")

# Map source files to object files in the build directory
OBJECTS := $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) \
           $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default Rule
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile C++ Source Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling C++: $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C Source Files (Specifically for glad.c)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling C: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean run

# Run the simulation
run: all
	@echo "Launching OpenGL Simulation..."
	./$(TARGET)