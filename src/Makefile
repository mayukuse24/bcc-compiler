all:
	flex -o scanner.cpp scanner.l
	bison -o parser.cpp --report=state -d parser.y
	clang++ -std=c++11 -g scanner.cpp parser.cpp ast.cpp llvmir.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -lfl -o bcc

clean:
	rm -rf scanner.cpp
	rm -rf parser.cpp parser.hpp parser.tab.c parser.output
