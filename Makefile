CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

all: main

main: main.o chip8.o screen.o sprite.o
	$(CXX) $(CXXFLAGS) -o main main.o chip8.o screen.o sprite.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp chip8.hpp screen.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

chip8.o: chip8.cpp screen.hpp
	$(CXX) $(CXXFLAGS) -c chip8.cpp

screen.o: screen.cpp screen.hpp
	$(CXX) $(CXXFLAGS) -c screen.cpp

sprite.o: sprite.cpp sprite.hpp
	$(CXX) $(CXXFLAGS) -c sprite.cpp

clean: 
	rm -f main.o chip8.o screen.o sprite.o main
