CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra

BUILD_DIR := build
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

OUT_DIR := $(BUILD_DIR)/$(CONFIG)

OBJECTS := $(patsubst src/%.cpp,$(OUT_DIR)/%.o,$(SOURCES))
EXE := $(OUT_DIR)/$(TARGET)

.PHONY: all debug release clean

all: $(EXE)

debug:
	$(MAKE) CONFIG=debug

release:
	$(MAKE) CONFIG=release

# Compilation
$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/%.o: src/%.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(BUILD_FLAGS) -MMD -MP -c $< -o $@

# Linking

$(EXE): $(OBJECTS) | $(OUT_DIR)
	$(CXX) $(OBJECTS) -o $@

# Cleanup
clean:
	rm -rf $(BUILD_DIR)

# Dependencies
DEPS := $(OBJECTS:.o=.d)

-include $(DEPS)