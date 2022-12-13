# Change -O2 to -g if need to debug
CFLAGS := -std=c++17 -fvisibility=hidden -lpthread -Wall -Wextra -O2 -fopenmp

CXX = g++


HEADERS := src/*.h

all: solver_sequential_v0 solver_sequential_v1 solver_sequential_v2 solver_parallel_v2

solver_sequential_v0: $(HEADERS) src/solver-sequential-v0.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-sequential-v0.cpp

solver_sequential_v1: $(HEADERS) src/solver-sequential-v1.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-sequential-v1.cpp

solver_sequential_v2: $(HEADERS) src/solver-sequential-v2.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-sequential-v2.cpp

solver_parallel_v2: $(HEADERS) src/solver-parallel-v2.cpp
	$(CXX) -o $@ $(CFLAGS) src/solver-parallel-v2.cpp

clean:
	rm -rf ./solver*
