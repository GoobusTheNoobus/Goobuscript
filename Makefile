CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra

BUILD_DIR := build
EXE_DIR := bin
SOURCES := $(wildcard src/*.cpp) # All .cpp files in src/ folder

TARGET := gs


# flags based on build config
DEBUG_FLAGS := -O0 -g3 -DDEBUG
RELEASE_FLAGS := -O3 -DNDEBUG


# decide build config
CONFIG ?= debug
ifeq ($(CONFIG),debug)
	BUILD_FLAGS := $(DEBUG_FLAGS)
else ifeq ($(CONFIG),release)
	BUILD_FLAGS := $(RELEASE_FLAGS)
else
	$(error Unknown CONFIG '$(CONFIG)'. Used debug or release)
endif


OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
EXE := $(EXE_DIR)/$(TARGET)

.PHONY: all debug release clean

all: $(EXE)

debug:
	$(MAKE) CONFIG=debug

release:
	$(MAKE) CONFIG=release

# Compilation
$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(BUILD_FLAGS) -c $< -o $@

# Linking

$(EXE): $(OBJECTS) | $(EXE_DIR)
	$(CXX) $(OBJECTS) -o $@

$(EXE_DIR):
	mkdir -p $@

# Cleanup
clean:
	rm -rf $(BUILD_DIR)