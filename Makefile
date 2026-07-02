CXX = g++
CXXFLAGS = -I include
LDFLAGS = -L lib -lSDL3_ttf -lSDL3_image -lSDL3_mixer -lSDL3

TARGET = game.exe

SRC = main.cpp      \
      menu.cpp      \
      AudioManager.cpp \
      player/Player.cpp \
      math/MathBox.cpp  \
      Background.cpp

all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	del $(TARGET)
