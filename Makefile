CXX = g++
CXXFLAGS = -Ilib -Iinclude -Os -std=c++23
TARGET = coin
SRC = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
