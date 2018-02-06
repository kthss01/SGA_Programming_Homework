#include "DrawImage2.h"
#include <cmath>

enum Direction { FRONT, BACK, LEFT, RIGHT,};

void imageSetup(DrawImage2* image, Rect rect); // 이미지 셋팅 함수
// 충돌 확인 함수
bool checkCollision(int x1, int y1, int x2, int y2, Direction dir); 
void removeCursor();	// 커서 지우는 함수

void main() {
	removeCursor();

	// 캐릭터 이미지
	Rect playerRect = { 100,100, 100,100 };
	DrawImage2* player = new DrawImage2;
	imageSetup(player, playerRect);

	// 돌 이미지
	Rect rockRect = { 300,300,100,100 };
	DrawImage2* rock = new DrawImage2;
	rock->AddImage(L"Images/stone.png", rockRect);

	int count = 0;
	int index = 0;
	int move = 10;
	int period = 4;

	Direction dir = FRONT;

	while (true) {
		player->Clear(0, 0, 0);
		Sleep(10);

		rock->Drawing(0, rockRect);

		switch (dir)
		{
		case FRONT:
			player->Drawing(index % 2, playerRect);
			break;
		case BACK:
			player->Drawing(index % 2 + 2, playerRect);
			break;
		case LEFT:
			player->Drawing(index % 2 + 4, playerRect);
			break;
		case RIGHT:
			player->Drawing(index % 2 + 6, playerRect);
			break;
		}

		char key = _getch();
		switch (key) {
		case 'A':
		case 'a':
			dir = LEFT;
			playerRect.X -= move;
			if (checkCollision(
				playerRect.X, playerRect.Y,
				rockRect.X, rockRect.Y, dir))
				rockRect.X -= move;
			count++;
			break;
		case 'D':
		case 'd':
			dir = RIGHT;
			playerRect.X += move;
			if (checkCollision(
				playerRect.X, playerRect.Y,
				rockRect.X, rockRect.Y, dir))
				rockRect.X += move;
			count++;
			break;
		case 'W':
		case 'w':
			dir = BACK;
			playerRect.Y -= move;
			if (checkCollision(
				playerRect.X, playerRect.Y,
				rockRect.X, rockRect.Y, dir))
				rockRect.Y -= move;
			count++;
			break;
		case 'S':
		case 's':
			dir = FRONT;
			playerRect.Y += move;
			if (checkCollision(
				playerRect.X, playerRect.Y,
				rockRect.X, rockRect.Y, dir))
				rockRect.Y += move;
			count++;
			break;
		}
		if (count % period == 1)
			index++;
	}

	delete player;
	delete rock;
}

void imageSetup(DrawImage2* image, Rect rect) {

	// 0 1
	image->AddImage(L"Images/front1.png", rect);
	image->AddImage(L"Images/front2.png", rect);

	// 2 3
	image->AddImage(L"Images/back1.png", rect);
	image->AddImage(L"Images/back2.png", rect);

	// 4 5
	image->AddImage(L"Images/left1.png", rect);
	image->AddImage(L"Images/left2.png", rect);
	
	// 6 7
	image->AddImage(L"Images/right1.png", rect);
	image->AddImage(L"Images/right2.png", rect);

}

bool checkCollision(int x1, int y1, int x2, int y2, Direction dir) {
	float minDistance = 100;
	float distanceError = 75;

	//float distance = sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
	// 이렇게 하면 무조건 붙어 다님
	
	// y1 == y2 이런 같은 위치일때만 밀게하면 조금만 어긋나면 뚫고 감

	// 오른쪽으로 밀때
	// 플레이어 위치 left(x1 < x2)	방향 right	x1 < x2, y1 == y2 (또는 비슷하게하기 위해 오차보다는 작을때)
	if (x1 < x2 && (abs(y1-y2) < distanceError)
		&& (abs(x2 - x1) < minDistance) && dir == RIGHT)
		return true;

	// 왼쪽으로 밀때
	// 플레이어 위치 right(x1 > x2)	방향 left	x1 > x2, y1 == y2
	if (x1 > x2 && (abs(y1 - y2) < distanceError)
		&& (abs(x1 - x2) < minDistance) && dir == LEFT)
		return true;

	// 아래로 밀때
	// 플레이어 위치 front(y1 > y2)	방향 back	y1 > y2, x1 == x2
	if (y1 > y2 && (abs(x1 - x2) < distanceError)
		&& (y1 - y2 < minDistance) && dir == BACK)
		return true;

	// 위로 밀때
	// 플레이어 위치 back(y1 < y2)	방향 front	y1 < y2, x1 == x2
	if (y1 < y2 && (abs(x1 - x2) < distanceError)
		&& (y2 - y1 < minDistance) && dir == FRONT)
		return true;

	return false;
}

void removeCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
