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
CXX_FLAGS := -g -Wall -Wextra -O3 -fno-strict-aliasing -Wswitch -DLL_VERSION='"$(VERSION)"'
LIB_FLAGS := -ldl -lpthread -lm
INCLUDE := -Iinclude -Ilibs # -Ilibs/imgui

# ================================
#          ImGui setup
# ================================
# IMGUI_DIR      := libs/imgui
# IMGUI_BACKENDS := $(IMGUI_DIR)/backends

# IMGUI_SRC := \
#     $(IMGUI_DIR)/imgui.cpp \
#     $(IMGUI_DIR)/imgui_draw.cpp \
#     $(IMGUI_DIR)/imgui_widgets.cpp \
#     $(IMGUI_DIR)/imgui_tables.cpp \
# #    $(IMGUI_DIR)/imgui_demo.cpp

# ifeq ($(PLATFORM),WINDOWS)
#     IMGUI_SRC += \
#         $(IMGUI_BACKENDS)/imgui_impl_win32.cpp \
#         $(IMGUI_BACKENDS)/imgui_impl_dx11.cpp
#     LIB_FLAGS += -ld3d11 -ld3dcompiler -ldxgi -lgdi32 -luser32 -ldwmapi \
# 				 -LC:/msys64/mingw64/lib
#     PLATFORM_INCLUDE := -I$(IMGUI_BACKENDS) -IC:/msys64/mingw64/include
#     CXX_FLAGS += -DPLATFORM_WINDOWS
# endif

# ifeq ($(PLATFORM),LINUX)
#     IMGUI_SRC += \
#         $(IMGUI_BACKENDS)/imgui_impl_sdl2.cpp \
#         $(IMGUI_BACKENDS)/imgui_impl_opengl3.cpp
#     LIB_FLAGS += -lGL -lSDL2 -ldl -lpthread `sdl2-config --libs`
#     PLATFORM_INCLUDE := -I$(IMGUI_BACKENDS)
#     CXX_FLAGS += -DPLATFORM_LINUX
# endif

# INCLUDE += -I$(IMGUI_DIR) $(PLATFORM_INCLUDE)

# ================================
#       Source directories
# ================================
SRC_DIR := src
OBJ_DIR := build
BIN := NoiseGoblin

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

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
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -o $@ $^ $(LIB_FLAGS)

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

# Clean results folder
cr:
	rm -rf results

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