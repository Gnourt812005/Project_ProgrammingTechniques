#include "Controller.h"

HANDLE Controller::consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
//hide the cursor
void Controller::showCursor(bool show)
{
	CONSOLE_CURSOR_INFO info = { 1, show };
	SetConsoleCursorInfo(consoleOutput, &info);
}

// Go to (X,Y) on console
void Controller::gotoXY(int pX, int pY)
{
	SetConsoleCursorPosition(consoleOutput, COORD{ (short)pX, (short)pY });
}

// set color for current backround and text
void Controller::setConsoleColor(int background, int text)
{
	SetConsoleTextAttribute(consoleOutput, background * 16 + text);
}

void Controller::clearConsole()
{
	system("cls");
}

// choose the button (step)
int Controller::getConsoleInput()
{
	int c = _getch(); // if arrow key is pressed then c = 224 (because it is not in ascii code) 
	// Arrow key
	if (c == 0 || c == 224)
	{
		switch (_getch())
		{
		case KEY_UP:				//lên
			return 2;
		case KEY_LEFT:				//trái
			return 3;
		case KEY_RIGHT:				//phải
			return 4;
		case KEY_DOWN:				//xuống
			return 5;
		default:				//nút khác
			return 0;
		}
	}
	else
	{
		if (c == KEY_ESC)                  //esc
			return 1;
		else if (c == 87 || c == 119) //W, w
			return 2;
		else if (c == 65 || c == 97)  //A, a
			return 3;
		else if (c == 68 || c == 100) //D, d
			return 4;
		else if (c == 83 || c == 115) //S, s
			return 5;
		else if (c == 13)             //Enter
			return 6;
		else if (c == 72 || c == 104) //H, h
			return 7;
		else if (c == 77 || c == 109) // M, m
			return 8;
		else
			return 0;                 //nút khác
	}
}

// play sound in each case
void Controller::playSound(int i)
{
	static vector<const wchar_t*> soundFile{L"move.wav", L"enter.wav", L"error.wav", L"placed.wav", L"win.wav",  L"background.wav", L"effect.wav" };
	PlaySound(soundFile[i], NULL, SND_FILENAME | SND_ASYNC);
}

// Set the tittle for terminal
void Controller::setConsoleTitle()
{
	SetConsoleTitle(L"\u0050\u0049\u004b\u0041\u0043\u0048\u0055"); // PIKACHU
}