CPP_FLAGS=-O2 -Wall -Werror -std=c++11

test: bin/test

bin/test: bin/main.o
	clang++ -o bin/test $(CPP_FLAGS) bin/main.o

bin/main.o: src/main.cpp src/vector.h
	clang++ -o bin/main.o -c $(CPP_FLAGS) src/main.cpp
