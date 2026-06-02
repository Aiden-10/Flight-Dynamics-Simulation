# =========================================================
# Native Windows Compiler Settings
# =========================================================
CXX      := g++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -Isrc 
LDFLAGS  := -static -lws2_32 -lmswsock -lpthread -luser32

# Directories
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

# Name of final program
TARGET    := $(BIN_DIR)/simulation.exe

# =========================================================
# Recursive Source Finding
# This looks inside src/ and ALL sub-folders for .cpp files
# =========================================================
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CPP_SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJECTS     := $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default Rule
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@echo Linking Windows Server $(TARGET)...
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile C++ Source Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(@D)" mkdir "$(@D)"
	@echo Compiling C++: $<...
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up 
clean:
	@echo Cleaning build artifacts...
	@if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)"

.PHONY: all clean run

# Run the simulation after building
run: all
	@echo Launching Simulation...
	$(TARGET)