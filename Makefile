
CXX := g++
CXXFLAGS := -g -I C:/msys64/mingw64/include
LDFLAGS := -L C:/msys64/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_DIR := code
SRC := $(addprefix $(SRC_DIR)/, \
    main_raylib.cpp GameManager.cpp Award.cpp DoorTile.cpp FloorTile.cpp \
    Food.cpp FoodTile.cpp Guard.cpp GuardTile.cpp MazeTile.cpp Player.cpp \
    Trap.cpp WallTile.cpp Menu.cpp)

TARGET := $(SRC_DIR)/game.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean