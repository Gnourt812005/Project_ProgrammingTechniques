#include "Menu.h"
int Menu::current_option;
const string Menu::options[8] = { "Play", "LeaderBoard", "Help", "Exit", "Easy", "  Medium   ", "Back", "Exit"};

void Menu::mainScreen()
{
// init variable and init the scrren
	Controller::showCursor(false);
	Controller::setConsoleTitle();
	unordered_map<string, void(*)()> function_map = {
		{options[0], playMenu},
		{options[1], leaderBoard},
		{options[2], helpScreen},
		{options[3], exitScreen},
		{options[4], playEasy},
		{options[5], playMedium},
		{options[6], goBack},
		{options[7], exitScreen}
	};
	//Controller::playSound(BACKGROUND_SOUND);
//
	bool loadMenu = 1; // check if need to reprint all the screen again
	while (true)
	{
		if (loadMenu) mainMenu();
		switch (Controller::getConsoleInput())
		{
			case 2:
				changeOption(0);
				loadMenu = 0;
				break;
			case 5:
				changeOption(1);
				loadMenu = 0;
				break;
			case 6:
				if (current_option == 0)
					loadMenu = 0;
				else
					loadMenu = 1;
				function_map[options[current_option]]();
				break;
			default:
				Controller::playSound(ERROR_SOUND);
				loadMenu = 0;
		}
	}

	Controller::clearConsole();
}

// print logo, creator, game
void Menu::printLogo()
{
// print the author
	Controller::gotoXY(38, 0);
	Controller::setConsoleColor(BLACK, RED);
	cout << "Ta Xuan Truong- Le Nguyen Nhat Truong";
	Controller::gotoXY(40, 1);
	cout << "23127506 - 23127506";
// print logo
	Controller::setConsoleColor(BLACK, YELLOW);
	unsigned char logo[] = R"(

		 ________  ___  ___  __    ________  ________  ___  ___  ___  ___     
		|\   __  \|\  \|\  \|\  \ |\   __  \|\   ____\|\  \|\  \|\  \|\  \    
		\ \  \|\  \ \  \ \  \/  /|\ \  \|\  \ \  \___|\ \  \\\  \ \  \\\  \   
		 \ \   ____\ \  \ \   __\_ \ \   __  \ \  \    \ \   __  \ \  \\\  \  
		  \ \  \___|\ \  \ \  \\ \  \ \  \ \  \ \  \____\ \  \ \  \ \  \\\  \ 
		   \ \__\    \ \__\ \__\\ \__\ \__\ \__\ \_______\ \__\ \__\ \_______\
		    \|__|     \|__|\|__| \|__|\|__|\|__|\|_______|\|__|\|__|\|_______|                                                                   
	)";
	cout << logo;
// print name of game
	Controller::gotoXY(38, 8);
	Controller::setConsoleColor(BLACK, BLUE);
	cout << R"(
  _____  _  _  ___   __  __    _  _____  ___  _  _  ___  _  _   ___    ___    _    __  __  ___ 
 |_   _|| || || __| |  \/  |  /_\|_   _|/ __|| || ||_ _|| \| | / __|  / __|  /_\  |  \/  || __|
   | |  | __ || _|  | |\/| | / _ \ | | | (__ | __ | | | | .` || (_ | | (_ | / _ \ | |\/| || _| 
   |_|  |_||_||___| |_|  |_|/_/ \_\|_|  \___||_||_||___||_|\_| \___|  \___|/_/ \_\|_|  |_||___|
	)";
}

// print the border around options
void Menu::printOptionsBoard()
{
	Controller::setConsoleColor(BLACK, WHITE);
	int left = 44; //48
	int top = 20;
	Controller::gotoXY(left, top);
	// = --> 205
	// || -->186
	// left top corner --> 201
	// right top corner --> 187
	// left down corner --> 200
	// right down corner --> 188
	putchar(201);
	for (int i = 1; i < 12; i++) putchar(205);
	putchar(187);

	for (int i = 2; i < 8; i += 2) {
		Controller::gotoXY(left, top + i);
		putchar(199);
		for (int i = 1; i < 12; i++) putchar(196);
		putchar(182);
	}
	for (int i = 1; i < 8; i += 2) {
		Controller::gotoXY(left, top + i);
		putchar(186);
		Controller::gotoXY(left + 12, top + i);
		putchar(186);
	}

	Controller::gotoXY(left, top + 8);
	putchar(200);
	for (int i = 1; i < 12; i++) putchar(205);
	putchar(188); 
}

void Menu::changeOption(bool direction) //0: lên, 1: xuống
{
	int top = 21;
// wrong move
	if ((direction == 0 && (current_option == 4 || current_option == 0))
		|| (direction == 1 && (current_option == 3 || current_option == 7)))
	{
		Controller::playSound(ERROR_SOUND);
		return;
	}
// reprint and unhighlight the last option
	Controller::setConsoleColor(BLACK, BRIGHT_WHITE);
	Controller::gotoXY(50 - (int)options[current_option].length() / 2, top + current_option % 4 * 2);
	cout << options[current_option];
//delete the arrow point to last option
		Controller::gotoXY(40, top + current_option % 4 * 2);
		putchar(32);
		Controller::gotoXY(60, top + current_option % 4 * 2);
		putchar(32);
		if (direction == 1) current_option++;
		else current_option--;
// reprint and highlight the current option
		Controller::playSound(MOVE_SOUND);
		Controller::setConsoleColor(BLACK, RED);
		Controller::gotoXY(50 - (int)options[current_option].length() / 2, top + current_option % 4 * 2);
		cout << options[current_option];
//add the arrow point to current option
		Controller::gotoXY(40, top + current_option % 4 * 2);
		putchar(175);
		Controller::gotoXY(60, top + current_option % 4 * 2);
		putchar(174);
}

// print the first menu (start menu) 
void Menu::mainMenu()
{
	Controller::clearConsole();
	printLogo();
	printOptionsBoard();
	current_option = 3;
	changeOption(0);
	changeOption(0);
	changeOption(0);
}

// print the second menu (choose stage)
void Menu::playMenu()
{
	current_option = 7;
	changeOption(0);
	changeOption(0);
	changeOption(0);
}

// choose easy mode
void Menu::playEasy()
{
	Game g(_EASY);
	g.setupGame();
	g.startGame();
}
//choose medium mode
void Menu::playMedium()
{
	Game g(_MEDIUM);
	g.setupGame();
	g.startGame();
}

// support function for exit, help, leader, print the given size rectangle
void Menu::printRectangle(int left, int top, int width, int height)
{
	// - -> 196
	// | -> 179
	// = --> 205
	// || -->186
	Controller::gotoXY(left, top);
	putchar(201); //218
	for (int i = 0; i < width; i++) putchar(205);
	putchar(187); //191

	int i = 0;
	for (; i < height; i++)
	{
		Controller::gotoXY(left, top + i + 1);
		putchar(186);
		Controller::gotoXY(left + width + 1, top + i + 1);
		putchar(186);
	}

	Controller::gotoXY(left, top + i);
	putchar(200); //192
	for (i = 0; i < width; i++) putchar(205);
	putchar(188); //217
}

// print the exit menu
void Menu::exitScreen()
{
	Controller::showCursor(false);
	Controller::clearConsole();
// print the board option
	Controller::setConsoleColor(BLACK, WHITE);
	Menu::printRectangle(34, 13, 35, 8);
	Menu::printRectangle(37, 18, 7, 2);
	Menu::printRectangle(60, 18, 6, 2);
// reprint logo
	Controller::gotoXY(0, 0);
	printLogo();
// init location and data
	Controller::gotoXY(42, 16);
	Controller::setConsoleColor(BLACK, WHITE);
	cout << "Do you want to exit?";
	string str[2] = { "Yes", "No" };
	int left[] = {35,40,47,58,63,69}, word[] = {32,32,175,174}, color[] = {WHITE, RED}, top = 19;
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
			Controller::gotoXY(left[choice * 3], top);        putchar(word[i * 2]);
			Controller::gotoXY(left[choice * 3 + 1], top);    cout << str[choice];
			Controller::gotoXY(left[choice * 3 + 2], top);    putchar(word[i * 2 + 1]);
			if (i == 0)
				choice = !choice;
			i++;
		}

		int key = Controller::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
			continue; //print1();
		else 
			if (key == 6)
			{
				if (!choice)
				{
					Controller::setConsoleColor(BLACK, WHITE);
					Controller::clearConsole();
					exit(0);
				}
				return;
			}
			else
				Controller::playSound(ERROR_SOUND);
	}
}


// print the help menu (go back only when press enter)
void Menu::helpScreen()
{
	Controller::showCursor(false);
	Controller::clearConsole();
	int left = 5, top = 2, width = 85, height = 23;
	int line1 = 6, line2 = 19, line3 = 20, line4 = 15;
// print the table and border inside
	Controller::setConsoleColor(BLACK, WHITE);
	printRectangle(left, top, width, height);
	Controller::gotoXY(left + 1, line1);
	for (int i = 0; i < width; i++) putchar(196);
	Controller::gotoXY(left + 1, line2);
	for (int i = 0; i < width; i++) putchar(196);
	Controller::gotoXY(left + 1, line4);
	for (int i = 0; i < width; i++) putchar(196);

	for (int i = 1; i < height; i++)
	{
		Controller::gotoXY(line3, top + i);
		putchar(179);
	}
	Controller::gotoXY(line3, line1);
	putchar(197);
	Controller::gotoXY(line3, line2);
	putchar(197);
// print the help menu
	Controller::setConsoleColor(BLACK, WHITE);
	Controller::gotoXY(left + 3, top + 2);
	cout << "Moves:";
	Controller::gotoXY(left + 20, top + 1);
	putchar(249); cout << "Up:    W, up arrow";
	Controller::gotoXY(left + 52, top + 1);
	putchar(249); cout << "Down:  S, down arrow";
	Controller::gotoXY(left + 20, top + 3);
	putchar(249); cout << "Left:  A, left arrow";
	Controller::gotoXY(left + 52, top + 3);
	putchar(249); cout << "Right: D, right arrow";
//////////////////////////////////////////////////////////
	Controller::gotoXY(left + 3, top + 8);
	cout << "Rules:";
	Controller::gotoXY(left + 17, top + 5);
	int left1 = left + 17;
	putchar(249); cout << " The Matching Game (commonly known as the Pikachu Puzzle Game)";
	Controller::gotoXY(left1, top + 6);
	cout << " includes a board of multiple cells, each of which presents a figure.";
	Controller::gotoXY(left1, top + 8);
	putchar(249); cout << " The player finds and matches a pair of cells that contain the same";
	Controller::gotoXY(left1, top + 9);
	cout << " figure and connect each other in some particular pattern.";
	Controller::gotoXY(left1, top + 11);
	putchar(249); cout << " A legal match will make the two cells disappear. The game ends when";
	Controller::gotoXY(left1, top + 12);
	cout << " all matching pairs are found.";
//////////////////////////////////////////////////////////
	Controller::gotoXY(left + 3, top + 15);
	cout << "Scoring:";
	Controller::setConsoleColor(BLACK, GREEN);
	Controller::gotoXY(left1 + 10, top + 14);
	putchar(249); cout << " I Matching: +1 BTC";
	Controller::gotoXY(left1 + 40, top + 14);
	putchar(249); cout << " L Matching: +2 BTC";
	Controller::gotoXY(left1 + 10, top + 15);
	putchar(249); cout << " Z Matching: +3 BTC";
	Controller::gotoXY(left1 + 40, top + 15);
	putchar(249); cout << " U Matching: +4 BTC";
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(left1 + 10, top + 16);
	putchar(249); cout << " Not Matched: -2 BTC";
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(left1 + 40, top + 16);
	putchar(249); cout << " Move suggestion: -2 BTC";
//////////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(left + 3, top + 20);
	cout << "Developers:";
	Controller::gotoXY(left + 31, top + 19);
	cout << "Dev 1: Ta Xuan Truong (23127506)";
	Controller::gotoXY(left + 31, top + 21);
	cout << "Dev 2: Le Nguyen Nhat Truong (21127645)";
//////////////////////////////////////////////////////////
// print the back button
	Controller::setConsoleColor(BLACK, WHITE);
	printRectangle(45, 27, 8, 2);
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(43, 28);
	putchar(175);
	Controller::gotoXY(48, 28);
	cout << "Back";
	Controller::gotoXY(56, 28); 
	putchar(174);
	while (Controller::getConsoleInput() != 6)
		Controller::playSound(ERROR_SOUND);
}

// print the leaderboard (go back only when press enter)
void Menu::leaderBoard()
{
	current_option = 0;
	Controller::clearConsole();
	
// print logo
	Controller::setConsoleColor(BLACK, YELLOW);
	cout << R"(
	  _      ______          _____  ______ _____  ____   ____          _____  _____  
	 | |    |  ____|   /\   |  __ \|  ____|  __ \|  _ \ / __ \   /\   |  __ \|  __ \ 
	 | |    | |__     /  \  | |  | | |__  | |__) | |_) | |  | | /  \  | |__) | |  | |
	 | |    |  __|   / /\ \ | |  | |  __| |  _  /|  _ <| |  | |/ /\ \ |  _  /| |  | |
	 | |____| |____ / ____ \| |__| | |____| | \ \| |_) | |__| / ____ \| | \ \| |__| |
	 |______|______/_/    \_\_____/|______|_|  \_\____/ \____/_/    \_\_|  \_\_____/                                                                 
	)";
/////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, WHITE);
	printRectangle(5, 8, 85, 17);
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(8, 9);
	cout << "STT";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 1; i < 17; i++)
	{
		Controller::gotoXY(13, 8 + i);
		putchar(179);
	}
	for (int i = 6; i < 13; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////	
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(18, 9);
	cout << "Name";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 1; i < 17; i++)
	{
		Controller::gotoXY(30, 8 + i);
		putchar(179);
	}
	for (int i = 14; i < 30; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(36, 9);
	cout << "ID";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 1; i < 17; i++)
	{
		Controller::gotoXY(45, 8 + i);
		putchar(179);
	}
	for (int i = 31; i < 45; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(52, 9);
	cout << "Class";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 1; i < 17; i++)
	{
		Controller::gotoXY(62, 8 + i);
		putchar(179);
	}
	for (int i = 46; i < 62; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(68, 9);
	cout << "Mode";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 1; i < 17; i++)
	{
		Controller::gotoXY(78, 8 + i);
		putchar(179);
	}
	for (int i = 63; i < 78; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(82, 9);
	cout << "Score";
	Controller::setConsoleColor(BLACK, WHITE);
	for (int i = 79; i < 91; i++)
	{
		Controller::gotoXY(i, 10);
		putchar(196);
	}
/////////////////////////////////////////////////////
//get data from file
	//vector <Player> p;
	Player p[100];
	int y = 11;
	int lines = 8;
	int n = 0;
	fstream fs("rank\\leaderboard.txt", ios::in);
	Controller::setConsoleColor(BLACK, WHITE);
	while (!fs.eof()) {
		getline(fs, p[n].playerName);
		getline(fs, p[n].playerID);
		getline(fs, p[n].className);
		getline(fs, p[n].mode);
		fs >> p[n].score;
		fs.ignore();
		n++;
	}
	fs.close();
// sort data
	for (int i = 0; i < n; i++) 
		for (int j = i + 1; j < n; j++) 
			if (p[j].score > p[i].score) 
				swap(p[i], p[j]);
// print data
	for (int i = 1; i < lines && i < n; i++) {
		Controller::gotoXY(9, y);
		cout << i;
		Controller::gotoXY(16, y);
		cout << p[i - 1].playerName;
		Controller::gotoXY(33, y);
		cout << p[i - 1].playerID;
		Controller::gotoXY(50, y);
		cout << p[i - 1].className;
		Controller::gotoXY(68, y);
		cout << p[i - 1].mode;
		Controller::gotoXY(84, y);
		cout << p[i - 1].score;
		y += 2;
	}

// print the back button
	Controller::setConsoleColor(BLACK, WHITE);
	printRectangle(45, 27, 8, 2);
	Controller::setConsoleColor(BLACK, RED);
	Controller::gotoXY(43, 28);
	putchar(175);
	Controller::gotoXY(48, 28);
	cout << "Back";
	Controller::gotoXY(56, 28);
	putchar(174);
	while (Controller::getConsoleInput() != 6)
		Controller::playSound(ERROR_SOUND);
}

void Menu::goBack() {
	return;
}
