CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

all: main

main: main.o chip8.o screen.o
	$(CXX) $(CXXFLAGS) -o main main.o chip8.o screen.o

main.o: main.cpp chip8.hpp screen.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

chip8.o: chip8.hpp screen.hpp
	$(CXX) $(CXXFLAGS) -c chip8.cpp

screen.o: screen.hpp
	$(CXX) $(CXXFLAGS) -c screen.cpp

clean: 
	rm -f main.o chip8.o screen.o