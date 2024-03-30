#ifndef GAME
#define GAME
#include "BoardView.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include "Menu.h"
#include "Controller.h"
#include <string>
#include <fstream>
#include <chrono>

// the start coordinate of the board

using namespace std;

struct PlayerGame{
	string name, id, idclass, mode;
	int score = 0, size;
	int interval = 0;
};

struct Game {
	PlayerGame player;
	int size_board;
	string mode_game;
	int current_coord_x, current_coord_y;
	bool is_playing;
	int locked_block;
	int remain_blocks;

	vector<pair<int, int>> pair_locked_block; //First: row - Second: column
	Game(int, string);
	bool checkMatchedPokemons(pair<int, int>, pair<int, int>);
	int checkIMatching(pair<int, int>, pair<int, int>, bool);
	bool checkLMatching(pair<int, int>, pair<int, int>, bool);
	bool checkZMatching(pair<int, int>, pair<int, int>, bool);
	bool checkUMatching(pair<int, int>, pair<int, int>, bool);
	bool checkMatching(pair<int, int>, pair<int, int>, bool);

	void setupGame();
	void setUpContinueGame();
	void startGame(bool);
	void printInterface(int);
	void saveData();
	void exitMenu();
	void saveGameNotComplete();
	int calTimeCompleteGame(int, int);
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

#endif

	
