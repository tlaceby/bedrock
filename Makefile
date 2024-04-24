# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Source directory
SRC_DIR = src

# Object directory
OBJ_DIR = obj

# Executable name
EXECUTABLE = bedrock

# Find all source files recursively in the source directory
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')

# Generate object file names from source files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

# Build target
build: $(EXECUTABLE)

# Run target
run: build
	./$(EXECUTABLE) run examples/test.br

# Phony targets
.PHONY: all clean build run