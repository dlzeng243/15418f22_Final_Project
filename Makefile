# Change -O2 to -g if need to debug
CFLAGS := -std=c++17 -fvisibility=hidden -lpthread -Wall -Wextra -O2

CXX = g++


HEADERS := src/*.h

all: solver-sequential solver-parallel

solver-sequential: $(HEADERS) src/solver-sequential.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-sequential.cpp

solver-parallel: $(HEADERS) src/solver-parallel.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-parallel.cpp

clean:
	rm -rf ./solver*
