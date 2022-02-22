#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
class room {
private:
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

	void repr() {
		cout << ' ' << type << ' ';
	}
};

const int SIZE = 15 + 2;
vector <room> maze;
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
			maze.push_back(room());
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

int scout(int cellId) {
	int TL = 1;
	maze[cellId].SetType(' ');
	//showMaze();
	if (maze[cellId - SIZE].GetType() == '+') {
		TL += scout(cellId - SIZE);
	}
	if (maze[cellId + 1].GetType() == '+') {
		TL += scout(cellId + 1);
	}
	if (maze[cellId + SIZE].GetType() == '+') {
		TL += scout(cellId + SIZE);
	}
	if (maze[cellId - 1].GetType()  == '+') {
		TL += scout(cellId - 1);
	}
	return TL;
}

int levelForm() {
	levelClear();
	levelWorm(SIZE / 2, 1, 0);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (maze[SIZE * i + j].GetType() == '*') {
				levelWorm(j, i, rand() % 4);
			}
		}
	}
	int depth = scout(SIZE + 1);
	if (depth < (SIZE-2)*(SIZE-2)/2) {
		levelForm();
	}
	else {
		maze[1] = room('-');
		return depth;
	}
}



int main() {
	srand(time(0));
	cout << levelForm() << endl;
	showMaze();
}