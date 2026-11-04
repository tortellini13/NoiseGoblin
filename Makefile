# ================================
#         OS detection
# ================================
ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM := LINUX
    else
        PLATFORM := UNKNOWN
    endif
endif

# ================================
#   Compiler and general flags
# ================================
VERSION := 0.1.0-alpha
CXX := g++
CXX_FLAGS := -g -Wall -Wextra -O3 -fno-strict-aliasing -Wswitch -DVERSION='"$(VERSION)"'
LIB_FLAGS :=
INCLUDE := -Iinclude -Ilibs

STATIC ?= 0

ifeq ($(STATIC),1)
    LDFLAGS := -static -static-libgcc -static-libstdc++
else
    LDFLAGS :=
endif

# ================================
# Platform-specific flags
# ================================
ifeq ($(PLATFORM),WINDOWS)
    LIB_FLAGS :=
    CXX_FLAGS += -DPLATFORM_WINDOWS
endif

ifeq ($(PLATFORM),LINUX)
    LIB_FLAGS := -ldl -lpthread -lm
    CXX_FLAGS += -DPLATFORM_LINUX
endif

# ================================
#       Source directories
# ================================
SRC_DIR := src
OBJ_DIR := build
BIN := NoiseGoblin

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) \
             libs/miniaudio.cpp

# Object files (works for files anywhere)
OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# ================================
#        Default target
# ================================
all: $(BIN)

# ================================
#  Link objects into executable
# ================================
$(BIN): $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LDFLAGS) $(INCLUDE) -o $@ $^ $(LIB_FLAGS)

# ================================
# Compile any .cpp file into build/
# ================================
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

# ================================
#             Clean
# ================================
clean:
	rm -rf $(OBJ_DIR) $(BIN) $(BIN).exe imgui.ini

# Clean configs and samples folder
cr:
	rm -rf configs samples

# Clean without libs
cl:
	rm -rf $(OBJ_DIR)/$(SRC_DIR)

# Run the program
run: $(BIN)
	./$(BIN)

# Clean without libs and run
clr: cl all run

# Full rebuild and run
rebuild: clean all run

# Run with GDB
gdb: $(BIN)
	gdb -ex run -ex bt ./$(BIN)

# Make without any external dependencies
static:
	$(MAKE) clean
	$(MAKE) STATIC=1 all