CPP_FLAGS=-O2 -Wall -Werror -std=c++11
TEST_FLAGS=-O2 -Werror -std=c++11 --pedantic

all: bin/main

bin/main: bin/main.o
	clang++ -o bin/main $(CPP_FLAGS) bin/main.o

bin/main.o: src/main.cpp
	clang++ -o bin/main.o -c $(CPP_FLAGS) src/main.cpp

test: bin/test

bin/test: bin/test.o
	clang++ -o bin/test $(TEST_FLAGS) bin/test.o

bin/test.o: src/test.cpp src/vector.h
	clang++ -o bin/test.o -c $(TEST_FLAGS) src/test.cpp
