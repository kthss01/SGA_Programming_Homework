#include "DrawImage.h"
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

void removeCursor();
void showCursor();

#define SLEEP_TIME 200
#define ESC 27

int minX = 0;
int minY = 150;
int maxX = 550;
int maxY = 550;

int x = 180;
int y = 550;
int chaMove = 20;

/*
	*여쭤보면 좋을 사항
	이미지 읽어왓을때 일정부분만 그리는방법
	-> 몰라서 다 잘라서 그림

	스트링 출력방법 (printf, cout은 문제있음)
	gotoxy() 함수있음 (커서 위치 제어) 이걸로 지정한 위치에 출력
	space 2칸 == enter 1칸 정도로 가진다 알고 하면됨
*/

void main() {
	removeCursor();
	
	DrawImage* face = new DrawImage();
	DrawImage* character = new DrawImage();
	DrawImage* map = new DrawImage();

	WCHAR* faceImage = L"Image/face.png";
	WCHAR* downImage[3] = 
	{ L"Image/Image_down1.png", L"Image/Image_down2.png", L"Image/Image_down3.png" };
	WCHAR* upImage[3] =
	{ L"Image/Image_up1.png", L"Image/Image_up2.png", L"Image/Image_up3.png" };
	WCHAR* leftImage[3] =
	{ L"Image/Image_left1.png", L"Image/Image_left2.png", L"Image/Image_left3.png" };
	WCHAR* rightImage[3] =
	{ L"Image/Image_right1.png", L"Image/Image_right2.png", L"Image/Image_right3.png" };
	WCHAR* mapImage = L"Image/map.png";

	face->AddImage(faceImage);
	map->AddImage(mapImage);

	char in = 0;
	int cnts[4] = { 0 };

	face->Drawing(0, 0);
	map->Drawing(0, 150);
	character->AddImage(downImage[0]);
	character->Drawing(x, y);

	while (in != ESC) {
		in = getch();
		character->Clear(0, 0, 0);

		face->Drawing(0, 0);
		map->Drawing(0, 150);
		//cout << x << y << endl;

		switch (in) {
		case 'w':
		case 'W':
			character->AddImage(upImage[cnts[0]]);
			y -= chaMove;
			if (y < minY)
				y = minY;
			character->Drawing(x, y);
			Sleep(SLEEP_TIME);

			cnts[0]++;
			if (cnts[0] == 3)
				cnts[0] = 0;	// 0,1,2 이미지만 있어서 3일때 다시 0으로
			cnts[1] = cnts[2] = cnts[3] = 0;	// 나머지는 초기화
			break;
		case 's':
		case 'S':
			character->AddImage(downImage[cnts[1]]);
			y += chaMove;
			if (y > maxY)
				y = maxY;
			character->Drawing(x, y);
			Sleep(SLEEP_TIME);
		
			cnts[1]++;
			if (cnts[1] == 3)
				cnts[1] = 0;	// 0,1,2 이미지만 있어서 3일때 다시 0으로
			cnts[0] = cnts[2] = cnts[3] = 0;	// 나머지는 초기화
			break;
		case 'a':
		case 'A':
			character->AddImage(leftImage[cnts[2]]);
			x -= chaMove;
			if (x < minX)
				x = minX;
			character->Drawing(x, y);
			Sleep(SLEEP_TIME);
			
			cnts[2]++;
			if (cnts[2] == 3)
				cnts[2] = 0;	// 0,1,2 이미지만 있어서 3일때 다시 0으로
			cnts[0] = cnts[1] = cnts[3] = 0;	// 나머지는 초기화
			break;
		case 'd':
		case 'D':
			character->AddImage(rightImage[cnts[3]]);
			x += chaMove;
			if (x > maxX)
				x = maxX;
			character->Drawing(x, y);
			Sleep(SLEEP_TIME);
			
			cnts[3]++;
			if (cnts[3] == 3)	// 0,1,2 이미지만 있어서 3일때 다시 0으로
				cnts[3] = 0;
			cnts[0] = cnts[2] = cnts[1] = 0;	// 나머지는 초기화
			break;
		default:
			break;
		}
	}

	// test
	/*int cnt = 0;
	while (true) {
		switch (cnt) {
		case 0:
			character->AddImage(L"Image/Image_down1.png");
			character->Drawing(100, 100);
			Sleep(SLEEP_TIME);
			character->Clear(0, 0, 0);
			break;
		case 1:
			character->AddImage(L"Image/Image_down2.png");
			character->Drawing(100, 100);
			Sleep(SLEEP_TIME);
			character->Clear(0, 0, 0);
			break;
		case 2:
			character->AddImage(L"Image/Image_down3.png");
			character->Drawing(100, 100);
			Sleep(SLEEP_TIME);
			character->Clear(0, 0, 0);
			break;
		}
		cnt++;
		if (cnt == 3)
			cnt = 0;
	}*/

	//Sleep(1000);

	// 커서 다시 보이게
	//showCursor();
}

void removeCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void showCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}