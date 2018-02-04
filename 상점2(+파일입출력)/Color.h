#pragma once

#include <Windows.h>

// getch() 를 위한 아스키 코드값 정의
#define ESC 27
#define SPACE 32
#define ENTER 13
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
#define SET_COLOR 2

class Color
{
public:
	Color();
	~Color();

	// 색깔 넣어주는 함수
	void SetColor(int color);
	// 원래 색으로 변경시켜주는 함수
	void SetDefaultColor();
};

