#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
using namespace std;
const int SIZE = 15 + 2;
const int CoinsOnLevel = 3;

class runner {
private:
	int PosX;
	int PosY;
	int coins = 0;
	int points = 0;
public:
	runner(int PosX, int PosY) {
		this->PosX = PosX;
		this->PosY = PosY;
	}
	int GetPosX() { return PosX; }
	int GetPosY() { return PosY; }
	int GetCoins() { return coins; }
	int GetPoints() { return points; }
	void SetPosX(int PosX) { this->PosX = PosX; }
	void SetPosY(int PosY) { this->PosY = PosY; }
	void SetPos(int cellId) {
		this->PosX = cellId % SIZE;
		this->PosY = cellId / SIZE;
	}
	void SetCoins(int coins) { this->coins = coins; }
	void AddCoins(int coins) { this->coins += coins; }
	void SetPoints(int points) { this->points = points; }
	void AddPoints(int points) { this->points += points; }
};
runner player(1, 1);

class room {
private:
	bool visible = false;
	char type = '*';
	int PosX;
	int PosY;
public:
	room() {}
	room(char type) { this->type = type; }
	room(int PosX, int PosY, char type) {
		this->PosX = PosX;
		this->PosY = PosY;
		this->type = type;
	}
	int GetPosX() { return PosX; }
	int GetPosY() { return PosY; }
	char GetType() { return type; }
	void SetPosX(int PosX) { this->PosX = PosX; }
	void SetPosY(int PosY) { this->PosY = PosY; }
	void SetType(char type) {
		if (this->type != 'X') {
			this->type = type;
		}
	}
	void SetVisible(bool visible) { this->visible = visible; }

	void repr() {
		if (false /*!visible*/) {
			cout << "   ";
			return void();
		}
		switch (type) {
		case ' ':
			if (PosX == player.GetPosX() and PosY == player.GetPosY()) {
				cout << "\x1b[1;44;32m" << " I " << "\x1b[0m";
			}
			else {
				cout << "\x1b[1;44m   \x1b[0m";
			}
			break;
		/*
		case 'X':
			cout << " X ";
			break;
		*/
		case '@':
			cout << "\x1b[44;33;1m @ \x1b[0m";
			break;
		default:
			cout << "\x1b[47m   \x1b[0m";
			break;
		}
	}
};


vector <room> maze;
vector <int> path;
void showMaze() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			maze[SIZE * i + j].repr();
		}
		cout << '\n';
	}
	cout << endl;
}


void levelClear() {
	maze.clear();
	for (int i = 0; i < SIZE; i++) {
		maze.push_back(room('X'));
	}
	for (int i = 0; i < SIZE - 2; i++) {
		maze.push_back('X');
		for (int j = 0; j < SIZE - 2; j++) {
			maze.push_back(room(j+1, i+1, '*'));
		}
		maze.push_back('X');
	}
	for (int i = 0; i < SIZE; i++) {
		maze.push_back(room('X'));
	}
}
void levelWorm(int x, int y, int enter) {
	vector <int> ways;
	if (maze[SIZE * (y - 1) + x].GetType() == '*') {
		ways.push_back(0);
	}
	if (maze[SIZE * y + x + 1].GetType() == '*') {
		ways.push_back(1);
	}
	if (maze[SIZE * (y + 1) + x].GetType() == '*') {
		ways.push_back(2);
	}
	if (maze[SIZE * y + x - 1].GetType() == '*') {
		ways.push_back(3);
	}
	maze[SIZE * y + x].SetType('@');
	//showMaze();
	maze[SIZE * y + x].SetType('+');
	if (ways.size() > 0) {
		int way = ways[rand() % ways.size()];
		if (way % 2 == 1) {
			maze[SIZE * (y + 1) + x].SetType('#');
			maze[SIZE * (y - 1) + x].SetType('#');
			if (enter % 2 == 0) {
				maze[SIZE * (y + enter - 1) + x].SetType('+');
			}
			else {
				maze[SIZE * y + x + 2 - enter].SetType('+');
			}
			levelWorm(x + 2 - way, y, (way + 2) % 4);
		}
		else {
			maze[SIZE * y + x + 1].SetType('#');
			maze[SIZE * y + x - 1].SetType('#');
			if (enter % 2 == 0) {
				maze[SIZE * (y + enter - 1) + x].SetType('+');
			}
			else {
				maze[SIZE * y + x + 2 - enter].SetType('+');
			}
			levelWorm(x, y + way - 1, (way + 2) % 4);
		}
	}
}
void scout(int cellId) {
	path.push_back(cellId);
	maze[cellId].SetType(' ');
	//showMaze();
	if (maze[cellId - SIZE].GetType() == '+') {
		scout(cellId - SIZE);
	}
	if (maze[cellId + 1].GetType() == '+') {
		scout(cellId + 1);
	}
	if (maze[cellId + SIZE].GetType() == '+') {
		scout(cellId + SIZE);
	}
	if (maze[cellId - 1].GetType()  == '+') {
		scout(cellId - 1);
	}
}
int levelForm() {
	levelClear();
	path.clear();
	levelWorm(SIZE / 2, 1, 0);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (maze[SIZE * i + j].GetType() == '*') {
				levelWorm(j, i, rand() % 4);
			}
		}
	}
	scout(SIZE + 1);
	if (path.size() < (SIZE - 2) * (SIZE - 2) / 2) {
		return levelForm();
	}
	int buf;
	for (int i = 0; i < CoinsOnLevel; i++) {
		buf = rand() % path.size();
		maze[path[buf]].SetType('@');
		path.erase(path.begin() + buf);
	}
	return path.size();
}


void showCells(int midCellId) {
	maze[midCellId].SetVisible(true);
	maze[midCellId + SIZE].SetVisible(true);
	maze[midCellId + 1].SetVisible(true);
	maze[midCellId - SIZE].SetVisible(true);
	maze[midCellId - 1].SetVisible(true);
}
void playerMove(char way) {
	switch (way) {
	case 'w':
		way = 0;
		break;
	case 'd':
		way = 1;
		break;
	case 's':
		way = 2;
		break;
	case 'a':
		way = 3;
		break;
	default:
		cout << "\n >";
		cin >> way;
		return playerMove(way);
	}
	int chosenCellId;
	int thisCellId = (SIZE * player.GetPosY() + player.GetPosX());
	if (way % 2 == 1) {
		chosenCellId = thisCellId + 2 - way;
	}
	else {
		chosenCellId = thisCellId + (way - 1) * SIZE;
	}
	switch (maze[chosenCellId].GetType()) {
	case ' ':
		player.SetPosX(maze[chosenCellId].GetPosX());
		player.SetPosY(maze[chosenCellId].GetPosY());
		break;
	case '@':
		player.SetPosX(maze[chosenCellId].GetPosX());
		player.SetPosY(maze[chosenCellId].GetPosY());
		player.AddCoins(1);
		maze[chosenCellId].SetType(' ');
		break;
	default:
		cout << "\nYou can't go into the wall :(\n" << endl;
	}
}


int main() {
	srand(time(0));
	cout << levelForm() << endl;
	char way;
	while (true) {
		showCells(player.GetPosY() * SIZE + player.GetPosX());
		showMaze();
		cout << "Now you're have " << player.GetCoins() << " coins.\n" << " >";
		cin >> way;
		system("Cls");
		for (int i = 0; i < 20; i++) {
			cout << '\n';
		}
		playerMove(way);
	}
}