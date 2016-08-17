CPP_FLAGS=-O2 -Wall -Werror -std=c++11

test: bin/test

bin/test: bin/memcheck.o bin/main.o
	clang++ -o bin/test $(CPP_FLAGS) bin/main.o bin/memcheck.o

bin/main.o: src/main.cpp src/vector.h src/vector_map.h src/memcheck.h src/test_map.h src/hash_map.h src/tree_map.h
	clang++ -o bin/main.o -c $(CPP_FLAGS) src/main.cpp

bin/memcheck.o: src/memcheck.h src/memcheck.cpp
	clang++ -o bin/memcheck.o -c $(CPP_FLAGS) src/memcheck.cpp

trace: bin/test
	./bin/test
	dot bin/trace*.dot -O -Tpng
	convert bin/trace*.dot.png +append bin/trace.png
	rm bin/trace*.dot.png
	rm bin/trace*.dot
