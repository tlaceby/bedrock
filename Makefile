CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra
SRC_DIR = src
OBJ_DIR = obj
EXECUTABLE = bedrock

# Find all source files recursively in the source directory
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

build: $(EXECUTABLE)
run: build
	./$(EXECUTABLE) run examples/test.br

.PHONY: all clean build run