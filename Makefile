CXX ?= g++

BUILD_DIR = build

SRC = main.cpp \
      menu.cpp \
      AudioManager.cpp \
      player/Player.cpp \
      math/MathBox.cpp \
      Background.cpp

CXXFLAGS += -I include
LDFLAGS  += -L lib -lSDL3_ttf -lSDL3_image -lSDL3_mixer -lSDL3

ifeq ($(OS),Windows_NT)
    TARGET_NAME = game.exe
    TARGET      = $(BUILD_DIR)\$(TARGET_NAME)
    MKDIR       = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RM          = rmdir /S /Q $(BUILD_DIR) 2>nul || exit 0
    RUN         = $(TARGET)
else
    TARGET_NAME = game
    TARGET      = $(BUILD_DIR)/$(TARGET_NAME)
    MKDIR       = mkdir -p $(BUILD_DIR)
    RM          = rm -rf $(BUILD_DIR)
    RUN         = ./$(TARGET)
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(MKDIR)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run: all
	$(RUN)

clean:
	$(RM)
