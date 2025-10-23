all:
	g++ main.cpp -o game.out
install:
	mkdir ./release/bin
	mv game.out ./release/bin/maze
