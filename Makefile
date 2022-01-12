all: compile link

compile:
	g++ -std=c++2a -I src/include -c main.cpp Map4x4.cpp other.cpp
link:
	g++ main.o other.o Map4x4.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
