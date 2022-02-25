#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>

#define SIZE_OF_THE_MAZE 14
#define COINS_ON_LEVEL 8
#define COINS_TO_ESCAPE 4
#define POINTS_FOR_COIN 100
#define TORCH_DURABILITY 100
#define FUEL_LOSS 2
#define FUEL_INDICATOR_LEN 50
#define FUEL_ON_LEVEL 2

#define COIN '$'
#define PIT '0'
#define PLAYER_CHAR 'I'
#define FUEL 'U'

#define KEY_UP 'w'
#define KEY_RIGHT 'd'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'

using namespace std;
const int SIZE = SIZE_OF_THE_MAZE + 4;
int torch_fuel = TORCH_DURABILITY;
int points;

class runner {
private:
	int PosX;
	int PosY;
	int coins = 0;
public:
	runner(int PosX, int PosY) {
		this->PosX = PosX;
		this->PosY = PosY;
	}
	int GetPosX() { return PosX; }
	int GetPosY() { return PosY; }
	int GetCoins() { return coins; }
	void SetPosX(int PosX) { this->PosX = PosX; }
	void SetPosY(int PosY) { this->PosY = PosY; }
	void SetPos(int cellId) {
		this->PosX = cellId % SIZE;
		this->PosY = cellId / SIZE;
	}
	void SetCoins(int coins) { this->coins = coins; }
	void AddCoins(int coins) { this->coins += coins; }
};
runner player(SIZE / 2, 1);

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

	
	void repr(bool isNearPlayer) {
		if (/*false*/ !isNearPlayer) {
			cout << "   ";
			return void();
		}
		switch (type) {
		case ' ':
			if (PosX == player.GetPosX() and PosY == player.GetPosY()) {
				cout << "\x1b[44;1;33m " << PLAYER_CHAR << " \x1b[0m";
			}
			else {
				cout << "\x1b[44m   \x1b[0m";
			}
			break;
		case COIN:
			cout << "\x1b[44;1;32m " << COIN << " \x1b[0m";
			break;
		case PIT:
			cout << "\x1b[44;1;36m " << PIT << " \x1b[0m";
			break;
		case FUEL:
			cout << "\x1b[44;1;31m " << FUEL << " \x1b[0m";
			break;
		default:
			cout << "\x1b[47m   \x1b[0m";
			break;
		}
	}
};
vector <room> maze;
vector <int> path;

bool nearPlayer(int PosX, int PosY) {
	/*if (abs(player.GetPosX() - PosX) < 2 and abs(player.GetPosY() - PosY) < 2 and (PosX == player.GetPosX() or PosY == player.GetPosY())) {
		return true;
	}*/
	if (abs(player.GetPosX() - PosX) < 2 and abs(player.GetPosY() - PosY) < 2 and ((maze[player.GetPosX() + SIZE * PosY].GetType() != '#' and maze[player.GetPosX() + SIZE * PosY].GetType() != 'X') or (maze[PosX + SIZE * player.GetPosY()].GetType() != '#' and maze[PosX + SIZE * player.GetPosY()].GetType() != 'X'))) {
		return true;
	}
	else if (player.GetPosX() - PosX == -2 and maze[PosY * SIZE + PosX - 1].GetType() != '#' and maze[PosY * SIZE + PosX - 1].GetType() != 'X' and player.GetPosY() == PosY) {
		return true;
	}
	else if (player.GetPosX() - PosX == 2 and maze[PosY * SIZE + PosX + 1].GetType() != '#' and maze[PosY * SIZE + PosX + 1].GetType() != 'X' and player.GetPosY() == PosY) {
		return true;
	}
	else if (player.GetPosY() - PosY == -2 and maze[PosX + (PosY - 1) * SIZE].GetType() != '#' and maze[PosX + (PosY - 1) * SIZE].GetType() != 'X' and player.GetPosX() == PosX) {
		return true;
	}
	else if (player.GetPosY() - PosY == 2 and maze[PosX + (PosY + 1) * SIZE].GetType() != '#' and maze[PosX + (PosY + 1) * SIZE].GetType() != 'X' and player.GetPosX() == PosX) {
		return true;
	}
	return false;
}
bool torchOFF(int PosX, int PosY) {
	if (abs(PosX - player.GetPosX()) <= 1 and PosY == player.GetPosY()) {
		return true;
	}
	else if (abs(PosY - player.GetPosY()) <= 1 and PosX == player.GetPosX()) {
		return true;
	}
	else {
		return false;
	}
}

void showTorchFuel(int len) {
	cout << endl << " \x1b[37m";
	/*for (int k = 0; k < len; k++) {
		cout << '_';
	}*/
	cout << "\n \x1b[33;41m";
	for (int point = 0; point < (torch_fuel * len) / TORCH_DURABILITY; point++) {
		cout << '-';
	}
	cout << "\x1b[0m";
	for (int point = 0; point < TORCH_DURABILITY - (torch_fuel * len / TORCH_DURABILITY); point++) {
		cout << ' ';
	}
	cout << "\x1b[37m \n ";
	/*for (int k = 0; k < len; k++) {
		cout << "‾";
	}*/
	cout  << "\x1b[0m" << endl;
}
void showMaze() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (torch_fuel > TORCH_DURABILITY / 3) {
				maze[SIZE * i + j].repr(nearPlayer(j, i));
			}
			else {
				maze[SIZE * i + j].repr(torchOFF(j, i));
			}
		}
		cout << '\n';
	}
	cout << endl;
}
void fallingAnimate(int high) {
	system("Cls");
	if (high < 0) { return void(); }
	for (int h = 15; h > 0; h--) {
		if (h == high) {
			cout << "\t\x1b[46m    \x1b[0;1;33m  " << PLAYER_CHAR << "  \x1b[46m    \x1b[0m\n";
		}
		else {
			cout << "\t\x1b[46m    \x1b[0m     \x1b[46m    \x1b[0m\n";
		}
	}
	fallingAnimate(high - 1);
}

int generateRoom(char type) {
	int buf = rand() % path.size();
	int cellId = path[buf];
	maze[cellId].SetType(type);
	path.erase(path.begin() + buf);
	return cellId;
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
	maze[SIZE * y + x].SetType(COIN);
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
int levelForm(int startCellId) {
	levelClear();
	path.clear();
	levelWorm(startCellId / SIZE, startCellId % SIZE, 0);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (maze[SIZE * i + j].GetType() == '*') {
				levelWorm(j, i, rand() % 4);
			}
		}
	}
	scout(startCellId);
	if (path.size() < (SIZE - 2) * (SIZE - 2) / 2) {
		return levelForm(startCellId);
	}
	path.erase(path.begin());
	for (int i = 0; i < COINS_ON_LEVEL; i++) { generateRoom(COIN); }
	for (int i = 0; i < FUEL_ON_LEVEL; i++) { generateRoom(FUEL); }
	return path.size();
}

void playerMove(char way) {
	switch (way) {
	case KEY_UP:
		way = 0;
		break;
	case KEY_RIGHT:
		way = 1;
		break;
	case KEY_DOWN:
		way = 2;
		break;
	case KEY_LEFT:
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
	case COIN:
		player.SetPosX(maze[chosenCellId].GetPosX());
		player.SetPosY(maze[chosenCellId].GetPosY());
		player.AddCoins(1);
		points += POINTS_FOR_COIN;
		maze[chosenCellId].SetType(' ');
		if (player.GetCoins() % COINS_TO_ESCAPE == 0 and player.GetCoins() != 0) {
			generateRoom(PIT);
		}
		break;
	case PIT:
		player.SetPosX(maze[chosenCellId].GetPosX());
		player.SetPosY(maze[chosenCellId].GetPosY());
		player.SetCoins(0);
		fallingAnimate(15);
		levelForm(chosenCellId);
		break;
	case FUEL:
		player.SetPosX(maze[chosenCellId].GetPosX());
		player.SetPosY(maze[chosenCellId].GetPosY());
		maze[chosenCellId].SetType(' ');
		torch_fuel = TORCH_DURABILITY;
	}
}

int main() {
	srand(time(0));
	levelForm(SIZE + SIZE / 2);
	char way;
	fallingAnimate(15);
	while (torch_fuel > 0) {
		showMaze();
		showTorchFuel(FUEL_INDICATOR_LEN);
		cout << "Now you're have " << points << " points.\n" << " >";
		cin >> way;
		system("Cls");
		playerMove(way);
		torch_fuel -= FUEL_LOSS;
	}
	cout << "LOOOSER";
}