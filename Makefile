all:
	g++ main.cpp -o a.out
install:
	mkdir ./release/bin
	mv a.out ./release/bin/maze
