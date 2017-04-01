all:
	mkdir -p bin
	clang++ -std=c++14 *.cpp -lstdc++ -o bin/vmvm

run:
	bin/vmvm run.vasm
