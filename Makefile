all: compile link

compile:
	g++ -std=c++2a -I src/include -c main.cpp other.cpp MapInterface.cpp
link:
	g++ main.o other.o MapInterface.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system

# Map4x4.o Map1x2.o Map2x2.o Map2x4.o
# Map1x2.cpp Map2x2.cpp Map2x4.cpp Map4x4.cpp