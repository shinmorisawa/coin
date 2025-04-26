CXX = g++
CXXFLAGS = -Os -std=c++20
TARGET = coin
SRC = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
