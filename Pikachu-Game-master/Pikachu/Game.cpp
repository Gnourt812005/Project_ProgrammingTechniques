#include "Game.h"

Game::Game(int mode)
{
	_mode = mode;
	_x = LEFT, _y = TOP;
	board = nullptr;
	isPlaying = true;
	_lockedBlock = 0;
	_lockedBlockPair.clear();
	_remainBlocks = _mode * _mode;
	score = 0;
}
/*
Game::~Game() {
	delete board;
	board = nullptr;
}*/

// ask player to filling profile
void Game::setupGame() {
	Controller::clearConsole();
	Controller::gotoXY(0, 0);
	Menu::printLogo();
//
	Controller::gotoXY(25, 15);
	Controller::setConsoleColor(BLACK, WHITE);
	cout << "Please enter your name shortly, under 10 characters!";
/////////////////////////////////////////////////////////////////
	Controller::gotoXY(35, 18);
	cout << "Enter your name:  ";
	Controller::showCursor(true);
	getline(cin, playerName);
/////////////////////////////////////////////////////////////////
	Controller::gotoXY(35, 20);
	cout << "Enter your ID:  ";
	getline(cin, playerID);
/////////////////////////////////////////////////////////////////
	Controller::gotoXY(35, 22);
	cout << "Enter your class's name:  ";
	getline(cin, className);
/////////////////////////////////////////////////////////////////
	if (_mode == 4)
		mode = "EASY";
	else
		mode = "MEDIUM";
	Controller::showCursor(false);
}

// Print the screen of game
void Game::printInterface(int create_new_data)
{
	board->createBackground();
	board->showBoard();
	if (create_new_data)
		board->buildBoardData();
	board->renderBoard();
	// print the profile of player
	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(59, 1, 33, 10);
	Menu::printRectangle(59, 12, 33, 10);
	Menu::printRectangle(60, 2, 31, 2);
	Controller::setConsoleColor(BLACK, WHITE);
	Controller::gotoXY(67, 3);
	cout << "PLAYER'S INFORMATION";
	///////////////////////////////////////////////////
	Controller::gotoXY(65, 5);
	if (playerName.length() == 0)
		playerName = "unknown";
	cout << "Player's name: " << playerName;
	///////////////////////////////////////////////////
	Controller::gotoXY(65, 7);
	if (playerID.length() == 0)
		playerID = "unknown";
	cout << "Player's ID: " << playerID;
	////////////////////////////////////////////////////
	Controller::gotoXY(65, 9);
	if (className.length() == 0)
		className = "unknown";
	cout << "Class: " << className;
	////////////////////////////////////////////////////

	Menu::printRectangle(60, 13, 31, 2);
	Controller::gotoXY(69, 14);
	cout << "GAME INFORMATION";
	Controller::gotoXY(65, 16);
	cout << "Moves:";
	Controller::gotoXY(65, 17);
	cout << "Current score:";
	Controller::gotoXY(80, 17);
	cout << score;

	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(59, 24, 33, 2);
	Menu::printRectangle(59, 27, 14, 2);
	Menu::printRectangle(78, 27, 14, 2);

	
	Controller::gotoXY(67, 25);
	cout << "M : Move suggestion";
	
	Controller::gotoXY(63, 28);
	cout << "H : Help";
	
	Controller::gotoXY(81, 28);
	cout << "Esc : Exit";
}

// start and control the game 
void Game::startGame() 
{
	Controller::clearConsole();
	while (isPlaying) {
		board = new BoardView(_mode, LEFT, TOP); // create a new data of board 
		_remainBlocks = _mode * _mode;
		score = 0;
		printInterface(1);
		_x = board->getXAt(0, 0);
		_y = board->getYAt(0, 0);
		Controller::gotoXY(_x, _y);
		board->selectedBlock(_x, _y, GREEN);
		Controller::gotoXY(_x, _y);
		
		while (_remainBlocks) {
// check if the board is error
			if (!isAvailableBlock(true)) {
				Game::announceReset();
				break;
				//startGame();
			}
// get the input
			switch (Controller::getConsoleInput())
			{
			case 0:
				Controller::playSound(ERROR_SOUND);
				break;
			case 1: //ESC
				//saveData();
				board->unselectedBlock(_x, _y);
				if (_lockedBlock == 1)
					board->unselectedBlock(_lockedBlockPair[0].first, _lockedBlockPair[0].second);
				Menu::exitScreen();
				Controller::clearConsole();
				printInterface(0);
				break;
			case 2: // UPARROW
				moveUp();
				break;
			case 3: // LEFTARROW
				moveLeft();
				break;
			case 4: // RIGHTARROW
				moveRight();
				break;
			case 5: // DOWNARROW
				moveDown();
				break;
			case 6: // ENTER
				lockBlock();
				break;
			case 7:
				board->unselectedBlock(_x, _y);
				if (_lockedBlock == 1) 
					board->unselectedBlock(_lockedBlockPair[0].first, _lockedBlockPair[0].second);
				Menu::helpScreen();
				Controller::clearConsole();
				printInterface(0);
				break;
			case 8:
				moveSuggestion();
				break;
			}
// if 2 block are chooosed then check 
			if (_lockedBlock == 2) {
				deleteBlock();
				board->selectedBlock(_x, _y, GREEN);
				if (_remainBlocks == 0) {
					Controller::setConsoleColor(BLACK, RED);
					Controller::gotoXY(69, 18);
					cout << "Game Announcement";
					Controller::setConsoleColor(BLACK, BLUE);
					Controller::gotoXY(67, 19);
					cout << "You have won the game.";
					Controller::setConsoleColor(BLACK, BLUE);
					Controller::gotoXY(69, 20);
					cout << "CONGRATULATIONS!";
					Controller::gotoXY(70, 21);
					cout << "Your score: " << score;
					Controller::playSound(WIN_SOUND);
					board->unselectedBlock(_x, _y);
					_x = board->getXAt(0, 0);
					_y = board->getYAt(0, 0);
					Controller::gotoXY(_x, _y);
					Sleep(5000);
				}
			}
		}
		saveData();
		askContinue();
		//return;
		delete board;
	}
}

// save the data of current player
void Game::saveData() {
	fstream fs("rank\\leaderboard.txt", ios::app);
	fs << playerName << '\n' << playerID << '\n' << className << '\n' << mode << '\n' << score << '\n';
	fs.close();
}

// print the announcement when player cannot move
void Game::announceReset()
{
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(69, 18);
	cout << "Game Announcement";
	Controller::gotoXY(64, 19);
	cout << "There are no more ways left!";
	Sleep(800);
	Controller::gotoXY(62, 21);
	cout << "Auto reset the board. Have fun!";
	Sleep(1000);
}
/////////////////////////////////////////////////////////////
void Game::moveRight()
{
	if (_x < board->getXAt(board->getSize() - 1, board->getSize() - 1))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		if (board->getCheckAtXY(_x, _y) == 0)
		{
			Controller::gotoXY(_x, _y);
		}
		_x += 8;

		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}
	else
		Controller::playSound(ERROR_SOUND);
	
}

void Game::moveLeft()
{
	if (_x > board->getXAt(0, 0))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		if (board->getCheckAtXY(_x, _y) == 0)
		{
			Controller::gotoXY(_x, _y);
		}
		_x -= 8;

		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}
	else
		Controller::playSound(ERROR_SOUND);
}

void Game::moveDown()
{
	if (_y < board->getYAt(board->getSize() - 1, board->getSize() - 1))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		if (board->getCheckAtXY(_x, _y) == 0)
		{
			Controller::gotoXY(_x, _y);
		}
		_y += 4;

		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}
	else
		Controller::playSound(ERROR_SOUND);
}

void Game::moveUp()
{
	if (_y > board->getYAt(0, 0))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(_x, _y) != _LOCK) {
			board->unselectedBlock(_x, _y);
		}

		if (board->getCheckAtXY(_x, _y) == 0)
		{
			Controller::gotoXY(_x, _y);
		}
		_y -= 4;

		if (board->getCheck(_x, _y) != _LOCK) {
			board->selectedBlock(_x, _y, GREEN);
		}
	}
	else
		Controller::playSound(ERROR_SOUND);
}

// lock the block player choose
void Game::lockBlock()
{
	Controller::playSound(ENTER_SOUND);
	if (board->getCheck(_x, _y) == _LOCK || board->getCheck(_x, _y) == _DELETE) {
		return;
	}
	board->lockBlock(_x, _y);
	
	_lockedBlockPair.push_back(pair<int, int>(_x, _y));
	_lockedBlock++;
}

//////////////////////////////////////////////////////////////
// check if 2 block are the same
bool Game::checkMatchedPokemons(pair<int, int> firstBlock, pair<int, int> secondBlock)
{
	return (board->getPokemons(firstBlock.first, firstBlock.second) == board->getPokemons(secondBlock.first, secondBlock.second));
}
// check if the straight line from block1 to block2 is clear
int Game::checkIMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
// if choose the same block return
	if (firstBlock.first == secondBlock.first && firstBlock.second == secondBlock.second) {
		return 2;
	}
// if the same line y (vertical) -> check value of x 
	if (firstBlock.second == secondBlock.second) {
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);

		for (int i = firstBlock.first + 8; i < secondBlock.first; i += 8) 
			if (board->getCheck(i, firstBlock.second) != _DELETE) 
				return 0;	
// support for 3 others matching 
		if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE
			|| board->getCheck(secondBlock.first, secondBlock.second) == _DELETE) {
			return 2;
		}

		if (checkMatchedPokemons(firstBlock,secondBlock))  {
			if (isChecking == false) {
				board->drawLineI(firstBlock, secondBlock);
				Sleep(200);
				board->deleteLineI(firstBlock, secondBlock);
			}
			return 1;
		}
	}
// if the same line x (horizontal) -> check value of y
	if (firstBlock.first == secondBlock.first) {
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

		for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4) 
			if (board->getCheck(firstBlock.first, i) != _DELETE) 
				return 0;
// support for 3 others matching
		if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE
			|| board->getCheck(secondBlock.first, secondBlock.second) == _DELETE) {
			return 2;
		}

		if (checkMatchedPokemons(firstBlock, secondBlock)) {
			if (isChecking == false) {
				board->drawLineI(firstBlock, secondBlock);
				Sleep(200);
				board->deleteLineI(firstBlock, secondBlock);
			}
			return 1;
		}
	}
	return 0;
}
// check if the corner of L is deleted, then check the straight line from block1 and block 2 to corner respectively 
bool Game::checkLMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	pair<int, int> Lcorner;
	Lcorner.first = firstBlock.first;
	Lcorner.second = secondBlock.second;
	if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE) {
		if (checkIMatching(Lcorner, firstBlock, isChecking) && checkIMatching(Lcorner, secondBlock, isChecking)) {
			if (isChecking == false) {
				board->drawLineL(firstBlock, secondBlock, Lcorner);
				Sleep(200);
				board->deleteLineL(firstBlock, secondBlock, Lcorner);
			}
			return 1;
		}
	}

	Lcorner.first = secondBlock.first;
	Lcorner.second = firstBlock.second;
	if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE) {
		if (checkIMatching(Lcorner, secondBlock, isChecking) && checkIMatching(Lcorner, firstBlock, isChecking)) {
			if (isChecking == false) {
				board->drawLineL(firstBlock, secondBlock, Lcorner);
				Sleep(200);
				board->deleteLineL(firstBlock, secondBlock, Lcorner);
			}
			return 1;
		}	
	}
	return 0;
}
// check if can find 2 corner make valid 3 I matching between 4 block
bool Game::checkZMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);

	pair<int, int> Zcorner1; // the first corner of L matching 
	pair<int, int> Zcorner2; // the second
// traversal from firstBlock.first to secondBlock.first 
// Find 2 cornerBlock that satisfy 3 I Matching 
	for (int i = firstBlock.first + 8; i < secondBlock.first; i += 8) 
		if (board->getCheck(i, firstBlock.second) == _DELETE) {
			if (board->getCheck(i, secondBlock.second) == _DELETE) {
				Zcorner1.first = i;	
				Zcorner1.second = firstBlock.second;
				Zcorner2.first = i;
				Zcorner2.second = secondBlock.second;
				if (checkIMatching(Zcorner1, Zcorner2, isChecking) &&
					checkIMatching(Zcorner2, secondBlock, isChecking)) {
					if (isChecking == false) {
						board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
						Sleep(200);
						board->deleteLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					}
					return 1;
				}
			}
		}
		else break;

	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);
// traversal from firstBlock.second to secondBlock.second 
// Find 2 cornerBlock that satisfy 3 I Matching 
	for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4) {
		if (board->getCheck(firstBlock.first, i) == _DELETE) {
			if (board->getCheck(secondBlock.first, i) == _DELETE) {
				Zcorner1.first = firstBlock.first;	
				Zcorner1.second = i;
				Zcorner2.first = secondBlock.first;
				Zcorner2.second = i;
				if (checkIMatching(Zcorner1, Zcorner2, isChecking) &&
					checkIMatching(Zcorner2, secondBlock, isChecking)) {
					if (isChecking == false) {
						board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
						Sleep(200);
						board->deleteLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					}
					return 1;
				}
			}
		}
		else break;
	}
	return 0;
}

bool Game::checkUMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	pair<int, int> Ucorner1;
	pair<int, int> Ucorner2;
	const int size = board->getSize();
	const int x = board->getXAt(0, 0);
	const int y = board->getYAt(0, 0);
	
	if (firstBlock.first == secondBlock.first)
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

// horizontal U matching on the left
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.first - 8; i >= x - 8; i -= 8) {
		Ucorner1.first = i;
		Ucorner1.second = firstBlock.second;
		Ucorner2.first = i;
		Ucorner2.second = secondBlock.second;

		if (i == x - 8) {
			Ucorner1.first = x;
			Ucorner1.second = firstBlock.second;
			Ucorner2.first = x;
			Ucorner2.second = secondBlock.second;
// the case 2 block at the margin
			if (Ucorner1.first == firstBlock.first && Ucorner2.first == secondBlock.first)
				return 1;
// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				(Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second))
				&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2)
					return 1;
			}
			else break;
		}
// the normal U matching
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE
			&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2
				&& checkIMatching(Ucorner1, Ucorner2, isChecking) == 2) {
				if (isChecking == false) {
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else break;
	}
// horizontal U matching on the right
	if (firstBlock.first < secondBlock.first)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.first + 8; i <= x + size * 8; i += 8) {
		Ucorner1.first = i;
		Ucorner1.second = firstBlock.second;
		Ucorner2.first = i;
		Ucorner2.second = secondBlock.second;

		if (i == x + size * 8) {
			Ucorner1.first = x + size * 8 - 8;
			Ucorner1.second = firstBlock.second;
			Ucorner2.first = x + size * 8 - 8;
			Ucorner2.second = secondBlock.second;
// the case 2 block at the margin
			if (Ucorner1.first == firstBlock.first && Ucorner2.first == secondBlock.first)
				return 1;
// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				(Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second))
				&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2)
					return 1;
			}
			else break;
		}
// the normal U matching
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE
			&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2
				&& checkIMatching(Ucorner1, Ucorner2, isChecking) == 2) {
				if (isChecking == false) {
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(1200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else break;
	}
	if (firstBlock.second == secondBlock.second)
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);	

// vertical U matching upward
	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.second - 4; i >= y - 4; i -= 4) {
		Ucorner1.first = firstBlock.first;
		Ucorner1.second = i;
		Ucorner2.first = secondBlock.first;
		Ucorner2.second = i;

		if (i == y - 4) {
			Ucorner2.first = secondBlock.first;
			Ucorner2.second = y;
			Ucorner1.first = firstBlock.first;
			Ucorner1.second = y;
// the case 2 block at the margin
			if (Ucorner1.second == firstBlock.second && Ucorner2.second == secondBlock.second)
				return 1;
// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE || 
				(Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second))
				&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2)
					return 1;
			}
			else break;
		}
// normal U matching
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE
			&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2
				&& checkIMatching(Ucorner1, Ucorner2, isChecking) == 2) {
				if (isChecking == false) {
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else break;
	}
// vertical U matching downward
	if (firstBlock.second < secondBlock.second)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.second + 4; i <= y + size * 4; i += 4) {
		Ucorner1.first = firstBlock.first;
		Ucorner1.second = i;
		Ucorner2.first = secondBlock.first;
		Ucorner2.second = i;

		if (i == y + size * 4) {
			Ucorner2.first = secondBlock.first;
			Ucorner2.second = y + size * 4 - 4;
			Ucorner1.first = firstBlock.first;
			Ucorner1.second = y + size * 4 - 4;
// the case 2 block at the margin
			if (Ucorner1.second == firstBlock.second && Ucorner2.second == secondBlock.second)
				return 1;
// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				(Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second))
				&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2)
					return 1;
			}
			else break;
		}
// normal U matching
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE
			&& board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE) {
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2
				&& checkIMatching(Ucorner1, Ucorner2, isChecking) == 2) {
				if (isChecking == false) {
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else break;
	}
	return 0;
}
// check if matching and update and print score
bool Game::checkMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	Controller::setConsoleColor(BLACK, WHITE);
	if (!checkMatchedPokemons(firstBlock, secondBlock)) {
		if (isChecking == false) {
			Controller::gotoXY(72, 16);
			cout << "Not Matched";
			score -= 2;

			Controller::gotoXY(80, 17);
			cout << score << " BTC ";
		}
		return 0;
	}
	if (checkIMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::gotoXY(72, 16);
			cout << "I Matching.";
			score += 1;
			Controller::gotoXY(80, 17);
			cout << score << " BTC ";
		}
		return 1;
	}
	if (checkLMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::gotoXY(72, 16);
			cout << "L Matching.";
			score += 2;
			Controller::gotoXY(80, 17);
			cout << score << " BTC ";
		}
		return 1;
	}
	if (checkZMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::gotoXY(72, 16);
			cout << "Z Matching.";
			score += 3;
			Controller::gotoXY(80, 17);
			cout << score << " BTC";
		}
		return 1;
	}
	if (checkUMatching(firstBlock, secondBlock, isChecking)) {
		if (isChecking == false) {
			Controller::gotoXY(72, 16);
			cout << "U Matching.";
			score += 4;
			Controller::gotoXY(80, 17);
			cout << score << " BTC";
		}
		return 1;
	}
	if (isChecking == false) {
		Controller::gotoXY(72, 16);
		cout << "Not Matched";
		score -= 2;
		Controller::gotoXY(80, 17);
		cout << score << " BTC ";
	}
	return 0;
}

// check if delete 2 lock block
void Game::deleteBlock() {
	_lockedBlock = 0;
	bool isChecking = false;
// if not matching then return the lock block to normal
	if (!checkMatching(_lockedBlockPair[0], _lockedBlockPair[1], isChecking)) {
		for (auto block : _lockedBlockPair)
			board->unselectedBlock(block.first, block.second);
		_lockedBlockPair.clear();
		return;
	}

// if matching then delete block
	for (auto block : _lockedBlockPair)
		board->deleteBlock(block.first, block.second);
	_lockedBlockPair.clear();
	_remainBlocks -= 2;

}

// bruteforce to check if there are any matching 
bool Game::isAvailableBlock(bool isChecking) {
	int size = board->getSize();
	pair<int, int> firstBlock;
	pair<int, int> secondBlock;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE)
				continue;
			for (int m =  i ; m < size; m++) {
				for (int n = 0; n < size; n++) {
					if (i == m && n <= j) continue;
					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);
					if(board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;
					if (checkMatching(firstBlock, secondBlock, isChecking)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

// ask for the next game
void Game::askContinue()
{
	Controller::clearConsole();
	Controller::gotoXY(0, 0);
	Menu::printLogo();
	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(34, 13, 35, 8);
	Menu::printRectangle(37, 18, 7, 2);
	Menu::printRectangle(60, 18, 6, 2);
	Controller::gotoXY(36, 16);
	Controller::setConsoleColor(BLACK, WHITE);
	cout << "Do you want to play another round?";
	string str[2] = { "Yes", "No" };
	int left[] = { 35,40,47,58,63,69 }, word[] = { 32,32,175,174 }, color[] = { RED, GREEN }, top = 19;
	bool choice = 1;
	auto print1 = [&]()
	{
		int i = 0;
		while (i < 2)
		{
			Controller::playSound(MOVE_SOUND);
			Controller::setConsoleColor(BLACK, color[i]);
			Controller::gotoXY(left[choice * 3], top);        putchar(word[i * 2]);
			Controller::gotoXY(left[choice * 3 + 1], top);    cout << str[choice];
			Controller::gotoXY(left[choice * 3 + 2], top);    putchar(word[i * 2 + 1]);
			if (!i++)
				choice = !choice;
		}
	};
	print1();
	while (true)
	{
		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
			print1();
		else if (key == 6)
		{
			if (!choice)
				isPlaying = true;
			else
				isPlaying = false;
			return;
		}
		else
			Controller::playSound(ERROR_SOUND);
	}
}

void Game::moveSuggestion() {
	bool isHelp = true;
	int size = board->getSize();
	pair<int, int> firstBlock;
	pair<int, int> secondBlock;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE) {
				continue;
			}
			for (int m = i; m < size; m++) {
				for (int n = 0; n < size; n++) {
					if (i == m && n <= j) continue;
					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;
					if (checkMatching(firstBlock, secondBlock, isHelp)) {
						if(isHelp) {
							board->selectedBlock(firstBlock.first, firstBlock.second, PURPLE);
							board->selectedBlock(secondBlock.first, secondBlock.second, PURPLE);
							Sleep(200);
							board->unselectedBlock(firstBlock.first, firstBlock.second);
							board->unselectedBlock(secondBlock.first, secondBlock.second);
							score -= 2;
							Controller::setConsoleColor(BLACK, RED);
							
							Controller::gotoXY(80, 17);
							cout << score << " BTC";
							
							return;
						}
					}
				}
			}
		}
	}
}