#include "Color.h"



Color::Color()
{
}


Color::~Color()
{
}

void Color::SetColor(int color)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	SetConsoleTextAttribute(hC, color);
}

void Color::SetDefaultColor()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	// 회색이 7번
	SetConsoleTextAttribute(hC, 7);
}
