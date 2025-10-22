# Detect platform
UNAME_S := $(shell uname -s)

# Common source files
SRC_DIR := code
SRC := $(addprefix $(SRC_DIR)/, \
    main_raylib.cpp GameManager.cpp Award.cpp DoorTile.cpp FloorTile.cpp \
    Food.cpp FoodTile.cpp Guard.cpp GuardTile.cpp MazeTile.cpp Player.cpp \
    Trap.cpp WallTile.cpp Menu.cpp)

# Platform-specific settings
ifeq ($(UNAME_S), Darwin)
    # macOS / iOS build
    CXX := clang++
    CXXFLAGS := -std=c++17 -I/opt/homebrew/include
    LDFLAGS := -L/opt/homebrew/lib -lraylib \
               -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
    TARGET := game_gui
else
    # Windows (assumes MSYS2 or similar environment)
    CXX := g++
    CXXFLAGS := -g -I C:/msys64/mingw64/include
    LDFLAGS := -L C:/msys64/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET := $(SRC_DIR)/game.exe
endif

# Default build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
ifeq ($(UNAME_S), Darwin)
	./$(TARGET)
else
	./$(TARGET)
endif

clean:
	rm -f $(TARGET)

.PHONY: all run clean