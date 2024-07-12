SHELL = bash
CC := g++

# Output directories
APP := text_search
BIN_DIR := bin
OBJECT_DIR := obj
GRAPH_DIR := graph
PYMODULE_BUILD_DIR := pymodule/build

# Source directories
INCLUDE_DIR := include
SOURCE_DIR := src

# Sources
CPP_HEADERS := $(wildcard $(INCLUDE_DIR)/*.hpp)
CPP_SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)

# Objects
COMPILE_FLAGS := -std=c++17 -Wall -Wunused-parameter -Wunreachable-code -Wextra -Wpedantic -Wconversion -I $(INCLUDE_DIR)
CPP_OBJECTS := $(patsubst $(SOURCE_DIR)/%, $(OBJECT_DIR)/%, ${CPP_SOURCES:.cpp=.o})

# Make flags
.PHONY: all clean

# Default
app: COMPILE_FLAGS += -O2 -flto
app: $(BIN_DIR)/$(APP)
debug: COMPILE_FLAGS += -g -Og
debug: $(BIN_DIR)/$(APP)
graph: $(BIN_DIR)/generate_graph

# Main: binary
$(BIN_DIR)/$(APP): $(CPP_OBJECTS)
	@echo -e ""
	@echo -e "\033[1m\033[92m### Building application\033[0m"
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(GRAPH_DIR)
	$(CC) $(CPP_OBJECTS) -o $@

# Main: objects
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(CPP_HEADERS)
	@echo -e "\033[92m### Compiling $<\033[0m"
	@mkdir -p $(OBJECT_DIR)
	$(CC) -c $(COMPILE_FLAGS) $< -o $@

# # Generate graph: object -> binary
# $(BIN_DIR)/generate_graph: $(OBJECT_DIR)/generate_graph.o
# 	mkdir -p $(BIN_DIR)
# 	$(CC) $(OBJECT_DIR)/generate_graph.o -o $(BIN_DIR)/generate_graph

# # Generate graph: source -> object
# $(OBJECT_DIR)/generate_graph.o: $(SOURCE_DIR)/generate_graph.cpp $(CPP_HEADERS)
# 	mkdir -p $(OBJECT_DIR)
# 	$(CC) -c $(COMPILE_FLAGS) $(SOURCE_DIR)/generate_graph.cpp -o $(OBJECT_DIR)/generate_graph.o

clean:
	@rm -rf $(BIN_DIR)/* $(OBJECT_DIR)/* $(GRAPH_DIR)/* $(PYMODULE_BUILD_DIR)
	@echo -e "\033[1m\033[93m### Project cleaned $<\033[0m"
	@echo -e "Cleared contents from $(BIN_DIR)"
	@echo -e "Cleared contents from $(OBJECT_DIR)"
	@echo -e "Cleared contents from $(GRAPH_DIR)"
	@echo -e "Cleared directory $(PYMODULE_BUILD_DIR)"
	@echo -e ""
