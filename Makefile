# Change -O2 to -g if need to debug
CFLAGS := -std=c++17 -fvisibility=hidden -lpthread -Wall -Wextra -O2

CXX = g++

TARGET = solver

HEADERS := src/*.h

all: $(TARGET)

$(TARGET): $(HEADERS) src/solver.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver.cpp

clean:
	rm -rf ./solver
