#pragma once
#include "BoardView.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include "Menu.h"
#include <string>
#include <fstream>

#define LEFT 5
#define TOP 5

using namespace std;

struct Game {
	BoardView* board;				// Create new board
	//char playerName[15] = "", playerID[9] = "", className[8] = "", mode[8] = "";
	string playerName, playerID, className, mode;
	int _mode;
	int _x, _y;
	bool isPlaying;
	int _lockedBlock;
	int _remainBlocks;
	int score;

	vector<pair<int, int>> _lockedBlockPair; //First: row - Second: column

	bool checkMatchedPokemons(pair<int, int>, pair<int, int>);
	int checkIMatching(pair<int, int>, pair<int, int>, bool);
	bool checkLMatching(pair<int, int>, pair<int, int>, bool);
	bool checkZMatching(pair<int, int>, pair<int, int>, bool);
	bool checkUMatching(pair<int, int>, pair<int, int>, bool);
	bool checkMatching(pair<int, int>, pair<int, int>, bool);

	Game(int);
	//~Game();

	void setupGame();
	void startGame();
	void printInterface(int);
	void saveData();

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	void lockBlock();
	void deleteBlock();
	bool isAvailableBlock(bool);
	void announceReset();
	void askContinue();
	void moveSuggestion();
};