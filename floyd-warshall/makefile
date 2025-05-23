CXX=g++
CXXFLAGS=-std=c++11 -Wall
TARGET=floyd.bin

all: $(TARGET)

$(TARGET): floyd.cpp
	$(CXX) $(CXXFLAGS) floyd.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
	