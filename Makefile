CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: cache_sim

cache_sim: main.o Cache.o
	$(CXX) $(CXXFLAGS) -o cache_sim main.o Cache.o

main.o: main.cpp Cache.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Cache.o: Cache.cpp Cache.h
	$(CXX) $(CXXFLAGS) -c Cache.cpp

clean:
	rm -f *.o cache_sim
