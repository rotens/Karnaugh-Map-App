all: compile link

compile:
	g++ -std=c++2a -I src/include -c main.cpp Map4x4.cpp other.cpp Map1x2.cpp Map2x2.cpp
link:
	g++ main.o other.o Map4x4.o Map1x2.o Map2x2.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
