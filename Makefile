CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TARGET = $(BUILD_DIR)/pathfinder

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

SRC_SUBDIRS = $(shell find $(SRC_DIR) -type d)
BUILD_SUBDIRS = $(SRC_SUBDIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

all: build

restart: rebuild
	@echo "\nRunning pathfinder..."
	./$(TARGET)

build: create_dirs $(TARGET)

create_dirs:
	mkdir -p $(BUILD_SUBDIRS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build

run: build
	./$(TARGET)

help:
	@echo "Available targets:"
	@echo "  build   - Build the pathfinder visualization"
	@echo "  clean   - Remove build files"
	@echo "  rebuild - Clean and rebuild"
	@echo "  run     - Build and run the program"
	@echo "  restart - Clean, rebuild and run"

.PHONY: all build clean rebuild run restart create_dirs help