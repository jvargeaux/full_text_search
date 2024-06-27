CC := g++

# Outputs
APP := text_search
BIN_DIR := bin
OBJECT_DIR := obj
GRAPH_DIR := graph

# Inputs
SOURCE_DIR := src
CPP_HEADERS := $(wildcard $(SOURCE_DIR)/*.hpp)
CPP_SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)

# COMPILE_FLAGS := -arch arm64

CPP_OBJECTS := $(patsubst $(SOURCE_DIR)/%, $(OBJECT_DIR)/%, ${CPP_SOURCES:.cpp=.o})

# Default
all: $(BIN_DIR)/$(APP)
graph: $(BIN_DIR)/generate_graph

# Main: object -> binary
$(BIN_DIR)/$(APP): $(CPP_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPP_OBJECTS) -o $@

# Main: source -> object
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(CPP_HEADERS)
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
	rm -r $(BIN_DIR) $(OBJECT_DIR) $(GRAPH_DIR)
