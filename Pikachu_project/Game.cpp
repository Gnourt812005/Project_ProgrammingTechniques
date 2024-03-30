#include "Game.h"
BoardView *board;
//constructor
Game::Game(int size, string mode)
{
	size_board = size;
	mode_game = mode;
	current_coord_x = LEFT, current_coord_y = TOP;
	// board = nullptr;
	is_playing = TRUE;
	locked_block = 0;
	pair_locked_block.clear();
	remain_blocks = size_board * size_board;
	// score = 0;
}

// ask player to filling profile
void Game::setupGame()
{
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
	Controller::showCursor(TRUE);
	getline(cin, player.name);
	if (player.name == "")
		player.name = "unknown";
	/////////////////////////////////////////////////////////////////
	Controller::gotoXY(35, 20);
	cout << "Enter your ID:  ";
	getline(cin, player.id);
	if (player.id == "")
		player.id = "unknown";
	/////////////////////////////////////////////////////////////////
	Controller::gotoXY(35, 22);
	cout << "Enter your class's name:  ";
	getline(cin, player.idclass);
	if (player.idclass == "")
		player.idclass = "unknown";
	/////////////////////////////////////////////////////////////////
	// if (size_board == 4)
	// 	player.mode = "EASY";
	// else
	// 	player.mode = "MEDIUM";
	player.mode = mode_game;
	player.size = size_board;
	Controller::showCursor(FALSE);
}
// get the last incomplete game data and setup
void Game::setUpContinueGame()
{
	ifstream file_in;
	file_in.open("continue\\continuegame.txt");
	if (!file_in)
		exit(1);
	int a;
	file_in.get();
	file_in.get();
	getline(file_in, player.name);
	getline(file_in, player.id);
	getline(file_in, player.idclass);
	getline(file_in, player.mode);
	file_in >> player.size;
	file_in >> player.score;
	file_in >> player.interval;
	size_board = player.size;
	remain_blocks = size_board * size_board;
	board = new BoardView(size_board, LEFT, TOP);
	if (player.mode == "HARD")
		board->blind = 1;
	else
		board->blind = 0;
	
	int count_delete = 0;
	for (int i = 0; i < board->getSize(); i++)
		for (int j = 0; j < board->getSize(); j++)
		{
			int temp;
			file_in >> temp;
			if (temp == 0)
			{
				((board->pBoard)[i][j]).setCheck(_DELETE);
				count_delete++;
			}
			else
				((board->pBoard)[i][j]).setPokemons(char(temp));
		}
	remain_blocks -= count_delete;
	file_in.close();
	ofstream file_out;
	file_out.open("continue\\continuegame.txt");
	file_out << 0;
	file_out.close();
}
// Print the screen of game
void Game::printInterface(int first_time)
{
	if (first_time == 1) {
		Controller::clearConsole();
		board->showBoard();
	}
	
	board->renderBoard();
	//print the profile of player
	if (first_time == 0)
		return;
	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(59, 1, 33, 10);
	Menu::printRectangle(59, 12, 33, 10);
	Menu::printRectangle(60, 2, 31, 2);
	Controller::setConsoleColor(BLACK, WHITE);
	Controller::gotoXY(67, 3);
	cout << "PLAYER'S INFORMATION";
	///////////////////////////////////////////////////
	Controller::gotoXY(65, 5);
	cout << "Player's name: " << player.name;
	///////////////////////////////////////////////////
	Controller::gotoXY(65, 7);
	cout << "Player's ID: " << player.id;
	////////////////////////////////////////////////////
	Controller::gotoXY(65, 9);
	cout << "Class: " << player.idclass;
	////////////////////////////////////////////////////
	Menu::printRectangle(60, 13, 31, 2);
	Controller::gotoXY(69, 14);
	cout << "GAME INFORMATION";
	Controller::gotoXY(65, 16);
	cout << "Moves:";
	Controller::gotoXY(65, 17);
	cout << "Current score:";
	Controller::gotoXY(80, 17);
	cout << player.score;

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
void Game::startGame(bool check_continue)
{
	Controller::clearConsole();
	while (is_playing)
	{
		if (check_continue == FALSE)
		{
			board = new BoardView(size_board, LEFT, TOP); // create a new data of board
			if (player.mode == "HARD");
				board->blind = TRUE;
			remain_blocks = size_board * size_board;
			player.score = 0;
			board->buildBoardData();
		}
		else
			check_continue = FALSE;
		board->createBackground();
		Game::printInterface(1);
		current_coord_x = board->getXAt(0, 0);
		current_coord_y = board->getYAt(0, 0);
		Controller::gotoXY(current_coord_x, current_coord_y);
		board->selectedBlock(current_coord_x, current_coord_y, GREEN);
		Controller::gotoXY(current_coord_x, current_coord_y);
		time_t time_start_playing, time_end_playing;
		time(&time_start_playing);
		// start playing
		while (remain_blocks && is_playing)
		{
			// check if the board is error
			if (!isAvailableBlock(TRUE))
			{
				Game::announceReset();
				break;
			}
			// get the input
			switch (Controller::getConsoleInput())
			{
			case 0:
				Controller::playSound(ERROR_SOUND);
				break;
			case 1: // ESC
				board->unselectedBlock(current_coord_x, current_coord_y);
				if (locked_block == 1)
					board->unselectedBlock(pair_locked_block[0].first, pair_locked_block[0].second);
				Game::exitMenu();
				if (is_playing)
				{
					Controller::clearConsole();
					printInterface(1);
					board->selectedBlock(current_coord_x, current_coord_y, GREEN);
				}
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
			case 7: // HELP
				board->unselectedBlock(current_coord_x, current_coord_y);
				if (locked_block == 1)
					board->unselectedBlock(pair_locked_block[0].first, pair_locked_block[0].second);
				Menu::helpScreen();
				Controller::clearConsole();
				printInterface(1);
				board->selectedBlock(current_coord_x, current_coord_y, GREEN);
				break;
			case 8:
				moveSuggestion();
				break;
			}
			// if 2 block are chooosed then check
			if (locked_block == 2)
			{
				deleteBlock();
				board->selectedBlock(current_coord_x, current_coord_y, GREEN);
			}
		}
		// check if win the game
		if (remain_blocks == 0)
		{
			time(&time_end_playing);
			player.interval += int(time_end_playing) - int(time_start_playing);//Game::calTimeCompleteGame(time_start_playing, time_end_playing);
			if (player.interval <= 30)
				player.score *= 2;
			else 
				if (player.interval <= 60)
					player.score *= 1.5;
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
			cout << "Your score: " << player.score;
			Controller::playSound(WIN_SOUND);
			board->unselectedBlock(current_coord_x, current_coord_y);
			current_coord_x = board->getXAt(0, 0);
			current_coord_y = board->getYAt(0, 0);
			Controller::gotoXY(current_coord_x, current_coord_y);
			Game::saveData();
			Sleep(4000);
			askContinue();
		}
		
		delete board;
	}
}

// save the data of current player
void Game::saveData()
{
	fstream fs("rank\\leaderboard.txt", ios::app);
	fs << player.name << '\n'
	   << player.id << '\n'
	   << player.idclass << '\n'
	   << player.mode << '\n'
	   << player.score << '\n'
	   << player.interval << '\n';
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
	string str[2] = {"Yes", "No"};
	int left[] = {35, 40, 47, 58, 63, 69}, word[] = {32, 32, 175, 174}, color[] = {GREEN, RED}, top = 19;
	bool choice = 1;
	// print1();
	while (TRUE)
	{
		int i = 0;
		while (i < 2)
		{
			Controller::playSound(MOVE_SOUND);
			Controller::setConsoleColor(BLACK, color[i]);
			Controller::gotoXY(left[choice * 3], top);
			putchar(word[i * 2]);
			Controller::gotoXY(left[choice * 3 + 1], top);
			cout << str[choice];
			Controller::gotoXY(left[choice * 3 + 2], top);
			putchar(word[i * 2 + 1]);
			if (!i++)
				choice = !choice;
		}
		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
		{
			continue;
		}
		else if (key == 6)
		{
			if (!choice)
				is_playing = TRUE;
			else
				is_playing = FALSE;
			return;
		}
		else
			Controller::playSound(ERROR_SOUND);
	}
}

// show the exit to menu option
void Game::exitMenu()
{
	Controller::showCursor(FALSE);
	Controller::clearConsole();
	// print the board option
	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(34, 13, 35, 8);
	Menu::printRectangle(37, 18, 7, 2);
	Menu::printRectangle(60, 18, 6, 2);
	// reprint logo
	// Controller::gotoXY(0, 0);
	// printLogo();
	// init location and data
	Controller::gotoXY(42, 16);
	Controller::setConsoleColor(BLACK, WHITE);
	cout << "Do you want to exit?";
	string str[2] = {"Yes", "No"};
	int left[] = {35, 40, 47, 58, 63, 69}, word[] = {32, 32, 175, 174}, color[] = {WHITE, RED}, top = 19;
	bool choice = 0; // 0 -> Yes; 1 -> No
	bool loop = 1;

	// choose the option
	// exit when key = 6(Enter) and choice = 0(Yes)
	while (loop)
	{
		// reprint the exit menu when move
		int i = 0;
		while (i < 2)
		{
			Controller::playSound(MOVE_SOUND);
			Controller::setConsoleColor(BLACK, color[i]);
			Controller::gotoXY(left[choice * 3], top);
			putchar(word[i * 2]);
			Controller::gotoXY(left[choice * 3 + 1], top);
			cout << str[choice];
			Controller::gotoXY(left[choice * 3 + 2], top);
			putchar(word[i * 2 + 1]);
			if (i == 0)
				choice = !choice;
			i++;
		}

		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
			continue;
		else if (key == 6)
		{
			if (!choice)
			{
				// Controller::setConsoleColor(BLACK, WHITE);
				// Controller::clearConsole();
				is_playing = FALSE;
				Game::saveGameNotComplete();
			}
			else
				is_playing = TRUE;
			return;
		}
		else
			Controller::playSound(ERROR_SOUND);
	}
}
// save the incomplete game
void Game::saveGameNotComplete()
{
	ofstream file_out;
	file_out.open("continue\\continuegame.txt");
	file_out << 1 << endl;
	file_out << player.name << '\n'
			 << player.id << '\n'
			 << player.idclass << '\n'
			 << player.mode << '\n'
			 << player.size << '\n'
			 << player.score << '\n';
	for (int i = 0; i < size_board; i++)
	{
		for (int j = 0; j < size_board; j++)
		{
			if (((board->pBoard)[i][j]).getCheck() == _DELETE)
				file_out << 0 << " ";
			else
				file_out << int(((board->pBoard)[i][j]).getPokemons()) << " ";
		}
		file_out << endl;
	}
	file_out.close();
}
/////////////////////////////////////////////////////////////
void Game::moveRight()
{
	if (current_coord_x < board->getXAt(board->getSize() - 1, board->getSize() - 1))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->unselectedBlock(current_coord_x, current_coord_y);
		/*
			* if (board->getCheckAtXY(current_coord_x, current_coord_y) == 0)
					Controller::gotoXY(current_coord_x, current_coord_y);
		*/
		current_coord_x += 8;

		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->selectedBlock(current_coord_x, current_coord_y, GREEN);
	}
	else
		Controller::playSound(ERROR_SOUND);
}

void Game::moveLeft()
{
	if (current_coord_x > board->getXAt(0, 0))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->unselectedBlock(current_coord_x, current_coord_y);
		/*
			* if (board->getCheckAtXY(current_coord_x, current_coord_y) == 0)
					Controller::gotoXY(current_coord_x, current_coord_y);
		*/
		current_coord_x -= 8;

		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->selectedBlock(current_coord_x, current_coord_y, GREEN);
	}
	else
		Controller::playSound(ERROR_SOUND);
}

void Game::moveDown()
{
	if (current_coord_y < board->getYAt(board->getSize() - 1, board->getSize() - 1))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->unselectedBlock(current_coord_x, current_coord_y);

		/*
			* if (board->getCheckAtXY(current_coord_x, current_coord_y) == 0)
					Controller::gotoXY(current_coord_x, current_coord_y);
		*/
		current_coord_y += 4;
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->selectedBlock(current_coord_x, current_coord_y, GREEN);
	}
	else
		Controller::playSound(ERROR_SOUND);
}

void Game::moveUp()
{
	if (current_coord_y > board->getYAt(0, 0))
	{
		Controller::playSound(MOVE_SOUND);
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->unselectedBlock(current_coord_x, current_coord_y);

		/*
			* if (board->getCheckAtXY(current_coord_x, current_coord_y) == 0)
					Controller::gotoXY(current_coord_x, current_coord_y);
		*/
		current_coord_y -= 4;
		if (board->getCheck(current_coord_x, current_coord_y) != _LOCK)
			board->selectedBlock(current_coord_x, current_coord_y, GREEN);
	}
	else
		Controller::playSound(ERROR_SOUND);
}

// lock the block player choose
void Game::lockBlock()
{
	Controller::playSound(ENTER_SOUND);
	if (board->getCheck(current_coord_x, current_coord_y) == _LOCK || board->getCheck(current_coord_x, current_coord_y) == _DELETE)
		return;

	board->lockBlock(current_coord_x, current_coord_y);

	pair_locked_block.push_back(pair<int, int>(current_coord_x, current_coord_y));
	locked_block++;
}

//////////////////////////////////////////////////////////////
// isCheking = TRUE --> this pass of checking is for isAvailableBlock or  moveSuggestion
// So do not draw line of matching or decrease score
// check if 2 block are the same
bool Game::checkMatchedPokemons(pair<int, int> firstBlock, pair<int, int> secondBlock)
{
	return (board->getPokemons(firstBlock.first, firstBlock.second) == board->getPokemons(secondBlock.first, secondBlock.second));
}
// check if the straight line from block1 to block2 is clear
int Game::checkIMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	// if choose the same block return
	if (firstBlock.first == secondBlock.first && firstBlock.second == secondBlock.second)
		return 2;

	// if the same line y (vertical) -> check value of x
	if (firstBlock.second == secondBlock.second)
	{
		if (firstBlock.first > secondBlock.first)
			swap(firstBlock, secondBlock);
		for (int i = firstBlock.first + 8; i < secondBlock.first; i += 8)
			if (board->getCheck(i, firstBlock.second) != _DELETE)
				return 0;
		// support for 3 others matching
		if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE || board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
			return 2;

		if (checkMatchedPokemons(firstBlock, secondBlock))
		{
			if (isChecking == FALSE)
			{
				board->drawLineI(firstBlock, secondBlock);
				Sleep(200);
				board->deleteLineI(firstBlock, secondBlock);
			}
			return 1;
		}
	}
	// if the same line x (horizontal) -> check value of y
	if (firstBlock.first == secondBlock.first)
	{
		if (firstBlock.second > secondBlock.second)
			swap(firstBlock, secondBlock);

		for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4)
			if (board->getCheck(firstBlock.first, i) != _DELETE)
				return 0;
		// support for 3 others matching
		if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE || board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
			return 2;

		if (checkMatchedPokemons(firstBlock, secondBlock))
		{
			if (isChecking == FALSE)
			{
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
	if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE)
	{
		if (checkIMatching(Lcorner, firstBlock, isChecking) && checkIMatching(Lcorner, secondBlock, isChecking))
		{
			if (isChecking == FALSE)
			{
				board->drawLineL(firstBlock, secondBlock, Lcorner);
				Sleep(200);
				board->deleteLineL(firstBlock, secondBlock, Lcorner);
			}
			return 1;
		}
	}

	Lcorner.first = secondBlock.first;
	Lcorner.second = firstBlock.second;
	if (board->getCheck(Lcorner.first, Lcorner.second) == _DELETE)
	{
		if (checkIMatching(Lcorner, secondBlock, isChecking) && checkIMatching(Lcorner, firstBlock, isChecking))
		{
			if (isChecking == FALSE)
			{
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
		if (board->getCheck(i, firstBlock.second) == _DELETE)
		{
			if (board->getCheck(i, secondBlock.second) == _DELETE)
			{
				Zcorner1.first = i;
				Zcorner1.second = firstBlock.second;
				Zcorner2.first = i;
				Zcorner2.second = secondBlock.second;
				if (checkIMatching(Zcorner1, Zcorner2, isChecking) &&
					checkIMatching(Zcorner2, secondBlock, isChecking))
				{
					if (isChecking == FALSE)
					{
						board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
						Sleep(200);
						board->deleteLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					}
					return 1;
				}
			}
		}
		else
			break;

	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);
	// traversal from firstBlock.second to secondBlock.second
	// Find 2 cornerBlock that satisfy 3 I Matching
	for (int i = firstBlock.second + 4; i < secondBlock.second; i += 4)
	{
		if (board->getCheck(firstBlock.first, i) == _DELETE)
		{
			if (board->getCheck(secondBlock.first, i) == _DELETE)
			{
				Zcorner1.first = firstBlock.first;
				Zcorner1.second = i;
				Zcorner2.first = secondBlock.first;
				Zcorner2.second = i;
				if (checkIMatching(Zcorner1, Zcorner2, isChecking) &&
					checkIMatching(Zcorner2, secondBlock, isChecking))
				{
					if (isChecking == FALSE)
					{
						board->drawLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
						Sleep(200);
						board->deleteLineZ(firstBlock, secondBlock, Zcorner1, Zcorner2);
					}
					return 1;
				}
			}
		}
		else
			break;
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

	//****************************************************************************************
	// horizontal U matching on the left
	if (firstBlock.first > secondBlock.first)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.first - 8; i >= x - 8; i -= 8)
	{
		// the case the loop go out the margin
		if (i == x - 8)
		{
			bool check = FALSE;
			Ucorner1.first = x;
			Ucorner1.second = firstBlock.second;
			Ucorner2.first = x;
			Ucorner2.second = secondBlock.second;
			// the case 2 block at the margin
			if (Ucorner1.first == firstBlock.first && Ucorner2.first == secondBlock.first)
				check = TRUE;
			// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				 (Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second)) &&
				board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
			{
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2) {
					check = TRUE;
				}
			}
			
			if (check == TRUE) {
				if (isChecking == FALSE)
				{
					Ucorner1.first = i;
					Ucorner1.second = firstBlock.second;
					Ucorner2.first = i;
					Ucorner2.second = secondBlock.second;
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
			else
				break;
		}
		// the normal U matching
		Ucorner1.first = i;
		Ucorner1.second = firstBlock.second;
		Ucorner2.first = i;
		Ucorner2.second = secondBlock.second;
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE && board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
		{
			if (checkIMatching(Ucorner1, firstBlock, isChecking) && checkIMatching(Ucorner2, secondBlock, isChecking) && checkIMatching(Ucorner1, Ucorner2, isChecking))
			{
				if (isChecking == FALSE)
				{
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else
			break;
	}
	//****************************************************************************************
	// horizontal U matching on the right
	if (firstBlock.first < secondBlock.first)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.first + 8; i <= x + size * 8; i += 8)
	{
		// the case the loop go out the margin
		if (i == x + size * 8)
		{
			bool check = FALSE;
			Ucorner1.first = x + size * 8 - 8;
			Ucorner1.second = firstBlock.second;
			Ucorner2.first = x + size * 8 - 8;
			Ucorner2.second = secondBlock.second;
			// the case 2 block at the margin
			if (Ucorner1.first == firstBlock.first && Ucorner2.first == secondBlock.first)
				check = TRUE;
			// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				 (Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second)) &&
				board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
			{
				if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2)
				{
				check = TRUE;
				}
			}
			
			if (check == TRUE) {
				if (isChecking == FALSE)
				{
					Ucorner1.first = i;
					Ucorner1.second = firstBlock.second;
					Ucorner2.first = i;
					Ucorner2.second = secondBlock.second;
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
			else
				break;
		}
		Ucorner1.first = i;
		Ucorner1.second = firstBlock.second;
		Ucorner2.first = i;
		Ucorner2.second = secondBlock.second;
		// the normal U matching
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE && board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
		{
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2 && checkIMatching(Ucorner1, Ucorner2, isChecking) == 2)
			{
				if (isChecking == FALSE)
				{
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else
			break;
	}
	//****************************************************************************************
	// vertical U matching upward
	if (firstBlock.second > secondBlock.second)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.second - 4; i >= y - 4; i -= 4)
	{
		// the case the loop go out the margin
		if (i == y - 4)
		{
			bool check = FALSE;
			Ucorner2.first = secondBlock.first;
			Ucorner2.second = y;
			Ucorner1.first = firstBlock.first;
			Ucorner1.second = y;
			// the case 2 block at the margin
			if (Ucorner1.second == firstBlock.second && Ucorner2.second == secondBlock.second)
				check = TRUE;
			// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				 (Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second)) &&
				board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
			{
			if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2) 
			{
				check = TRUE;
			}
			}
			
			if (check == TRUE) {
				if (isChecking == FALSE)
				{
					Ucorner1.first = firstBlock.first;
					Ucorner1.second = i;
					Ucorner2.first = secondBlock.first;
					Ucorner2.second = i;
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
			else
				break;
		}
		// normal U matching
		Ucorner1.first = firstBlock.first;
		Ucorner1.second = i;
		Ucorner2.first = secondBlock.first;
		Ucorner2.second = i;
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE && board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
		{
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2 && checkIMatching(Ucorner1, Ucorner2, isChecking) == 2)
			{
				if (isChecking == FALSE)
				{
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else
			break;
	}
	//****************************************************************************************
	// vertical U matching downward
	if (firstBlock.second < secondBlock.second)
		swap(firstBlock, secondBlock);
	for (int i = firstBlock.second + 4; i <= y + size * 4; i += 4)
	{
		bool check = FALSE;
		// the case the loop go out the margin
		if (i == y + size * 4)
		{
			Ucorner2.first = secondBlock.first;
			Ucorner2.second = y + size * 4 - 4;
			Ucorner1.first = firstBlock.first;
			Ucorner1.second = y + size * 4 - 4;
			// the case 2 block at the margin
			if (Ucorner1.second == firstBlock.second && Ucorner2.second == secondBlock.second)
				check = TRUE;
			// the case 2 block don't need be at the margin but the U matching need to go out the margin
			if ((board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE ||
				 (Ucorner1.first == firstBlock.first && Ucorner1.second == firstBlock.second)) &&
				board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
			{
				if (checkIMatching(firstBlock, Ucorner1, isChecking) == 2 && checkIMatching(secondBlock, Ucorner2, isChecking) == 2) 
				{
					check = TRUE;
				}
			}
			
			if (check == TRUE) {
				if (isChecking == FALSE)
				{
					Ucorner1.first = firstBlock.first;
					Ucorner1.second = i;
					Ucorner2.first = secondBlock.first;
					Ucorner2.second = i;
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
			else
				break;
		}
		// normal U matching
		Ucorner1.first = firstBlock.first;
		Ucorner1.second = i;
		Ucorner2.first = secondBlock.first;
		Ucorner2.second = i;
		if (board->getCheck(Ucorner1.first, Ucorner1.second) == _DELETE && board->getCheck(Ucorner2.first, Ucorner2.second) == _DELETE)
		{
			if (checkIMatching(Ucorner1, firstBlock, isChecking) == 2 && checkIMatching(Ucorner2, secondBlock, isChecking) == 2 && checkIMatching(Ucorner1, Ucorner2, isChecking) == 2)
			{
				if (isChecking == FALSE)
				{
					board->drawLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
					Sleep(200);
					board->deleteLineU(firstBlock, secondBlock, Ucorner1, Ucorner2);
				}
				return 1;
			}
		}
		else
			break;
	}
	return 0;
}
// check if matching and update and print score
bool Game::checkMatching(pair<int, int> firstBlock, pair<int, int> secondBlock, bool isChecking)
{
	Controller::setConsoleColor(BLACK, WHITE);
	if (!checkMatchedPokemons(firstBlock, secondBlock))
	{
		if (isChecking == FALSE)
		{
			Controller::gotoXY(72, 16);
			cout << "Not Matched";
			player.score -= 2;

			Controller::gotoXY(80, 17);
			cout << player.score << " BTC ";
		}
		return 0;
	}
	if (checkIMatching(firstBlock, secondBlock, isChecking))
	{
		if (isChecking == FALSE)
		{
			Controller::gotoXY(72, 16);
			cout << "I Matching.";
			player.score += 1;
			Controller::gotoXY(80, 17);
			cout << player.score << " BTC ";
		}
		return 1;
	}
	if (checkLMatching(firstBlock, secondBlock, isChecking))
	{
		if (isChecking == FALSE)
		{
			Controller::gotoXY(72, 16);
			cout << "L Matching.";
			player.score += 2;
			Controller::gotoXY(80, 17);
			cout << player.score << " BTC ";
		}
		return 1;
	}
	if (checkZMatching(firstBlock, secondBlock, isChecking))
	{
		if (isChecking == FALSE)
		{
			Controller::gotoXY(72, 16);
			cout << "Z Matching.";
			player.score += 3;
			Controller::gotoXY(80, 17);
			cout << player.score << " BTC";
		}
		return 1;
	}
	if (checkUMatching(firstBlock, secondBlock, isChecking))
	{
		if (isChecking == FALSE)
		{
			Controller::gotoXY(72, 16);
			cout << "U Matching.";
			player.score += 4;
			Controller::gotoXY(80, 17);
			cout << player.score << " BTC";
		}
		return 1;
	}
	if (isChecking == FALSE)
	{
		Controller::gotoXY(72, 16);
		cout << "Not Matched";
		player.score -= 2;
		Controller::gotoXY(80, 17);
		cout << player.score << " BTC ";
	}
	return 0;
}
// check if delete 2 lock block
void Game::deleteBlock()
{
	locked_block = 0;
	bool isChecking = FALSE;
	// if not matching then return the lock block to normal
	if (!checkMatching(pair_locked_block[0], pair_locked_block[1], isChecking))
		for (auto block : pair_locked_block)
			board->unselectedBlock(block.first, block.second);
	else
	{
		// if matching then delete block
		for (auto block : pair_locked_block)
			board->deleteBlock(block.first, block.second);
		remain_blocks -= 2;
		if (player.mode == "MEDIUM") {
			if (pair_locked_block[0].first > pair_locked_block[1].first)
				swap(pair_locked_block[0], pair_locked_block[1]);
			for (auto block : pair_locked_block)
				board->slideBlock(block);
			Game::printInterface(0);
		}
	}
	pair_locked_block.clear();
}

// bruteforce to check if there are any matching
bool Game::isAvailableBlock(bool isChecking)
{
	int size = board->getSize();
	pair<int, int> firstBlock;
	pair<int, int> secondBlock;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE)
				continue;
			for (int m = i; m < size; m++)
				for (int n = 0; n < size; n++)
				{
					if (i == m && n <= j)
						continue;
					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;
					if (checkMatching(firstBlock, secondBlock, isChecking))
						return TRUE;
				}
		}
	return FALSE;
}

// bruteforce to check which pair of blocks are matching
// and highlighting them
void Game::moveSuggestion()
{
	bool isHelp = TRUE;
	int size = board->getSize();
	pair<int, int> firstBlock;
	pair<int, int> secondBlock;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			firstBlock.first = board->getXAt(i, j);
			firstBlock.second = board->getYAt(i, j);
			if (board->getCheck(firstBlock.first, firstBlock.second) == _DELETE)
				continue;
			for (int m = i; m < size; m++)
				for (int n = 0; n < size; n++)
				{
					if (i == m && n <= j)
						continue;
					secondBlock.first = board->getXAt(m, n);
					secondBlock.second = board->getYAt(m, n);
					if (board->getCheck(secondBlock.first, secondBlock.second) == _DELETE)
						continue;
					if (checkMatching(firstBlock, secondBlock, isHelp))
					{
						// highlight 2 suggestive blocks
						board->selectedBlock(firstBlock.first, firstBlock.second, PURPLE);
						board->selectedBlock(secondBlock.first, secondBlock.second, PURPLE);
						Sleep(200);
						board->unselectedBlock(firstBlock.first, firstBlock.second);
						board->unselectedBlock(secondBlock.first, secondBlock.second);
						player.score -= 2;
						Controller::setConsoleColor(BLACK, RED);
						Controller::gotoXY(80, 17);
						cout << player.score << " BTC";
						return;
					}
				}
		}
}

int Game::calTimeCompleteGame(int start, int end) {
	return end - start;
}
