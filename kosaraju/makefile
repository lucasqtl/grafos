CXX=g++
CXXFLAGS=-std=c++11 -Wall
TARGET=kosaraju.bin

all: $(TARGET)

$(TARGET): kosaraju.cpp
	$(CXX) $(CXXFLAGS) kosaraju.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
	