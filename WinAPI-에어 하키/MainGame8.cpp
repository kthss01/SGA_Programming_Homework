#include "stdafx.h"
#include "MainGame8.h"


MainGame8::MainGame8()
{
}


MainGame8::~MainGame8()
{
}

void MainGame8::TagInit()
{
	ball.x = WINSIZEX / 2;
	ball.y = WINSIZEY / 2;
	ball.speed = 0;
	ball.width = 50;
	ball.height = 50;
	ball.rc = RectMakeCenter(ball.x, ball.y, ball.width, ball.height);

	player[0].x = WINSIZEX / 2 - 300;
	player[0].y = WINSIZEY / 2;
	player[0].speed = 0;
	player[0].width = 50;
	player[0].height = 50;
	player[0].rc = RectMakeCenter(
		player[0].x, player[0].y, player[0].width, player[0].height);

	player[1].x = WINSIZEX / 2 + 300;
	player[1].y = WINSIZEY / 2;
	player[1].speed = 0;
	player[1].width = 50;
	player[1].height = 50;
	player[1].rc = RectMakeCenter(
		player[1].x, player[1].y, player[1].width, player[1].height);
}

HRESULT MainGame8::Init()
{
	GameNode::Init();

	field = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 750, 600);

	TagInit();

	score[0].x = field.left - 25;
	score[0].y = field.top + (field.bottom - field.top) / 2;
	score[0].score = 0;
	score[0].rc = RectMakeCenter(score[0].x, score[0].y, 50, 200);

	score[1].x = field.right + 25;
	score[1].y = field.top + (field.bottom - field.top) / 2;
	score[1].score = 0;
	score[1].rc = RectMakeCenter(score[1].x, score[1].y, 50, 200);

	power = 0.25f;

	prevTime = GetTickCount();

	isMove = false;

	angle = 0;

	speedLimit = 30;

	return S_OK;
}

void MainGame8::Release()
{
	GameNode::Release();
}

void MainGame8::Update()
{
	GameNode::Update();
	
	// player0 Control
	if (INPUT->GetKey('A') && player[0].rc.left > field.left) {
		player[0].speed += power;

		if (player[0].speed > speedLimit)
			player[0].speed = speedLimit;

		player[0].x -= player[0].speed;
		player[0].rc = RectMakeCenter(
			player[0].x, player[0].y, player[0].width, player[0].height);
	}

	if (INPUT->GetKey('D') && player[0].rc.right <
		(field.left + (field.right - field.left) / 2)) {
		player[0].speed += power;

		if (player[0].speed > speedLimit)
			player[0].speed = speedLimit;

		player[0].x += player[0].speed;
		player[0].rc = RectMakeCenter(
			player[0].x, player[0].y, player[0].width, player[0].height);
	}

	if (INPUT->GetKey('W') && player[0].rc.top > field.top) {
		player[0].speed += power;

		if (player[0].speed > speedLimit)
			player[0].speed = speedLimit;

		player[0].y -= player[0].speed;
		player[0].rc = RectMakeCenter(
			player[0].x, player[0].y, player[0].width, player[0].height);
	}

	if (INPUT->GetKey('S') && player[0].rc.bottom < field.bottom) {
		player[0].speed += power;

		if (player[0].speed > speedLimit)
			player[0].speed = speedLimit;

		player[0].y += player[0].speed;
		player[0].rc = RectMakeCenter(
			player[0].x, player[0].y, player[0].width, player[0].height);
	}

	// player1 Control
	if (INPUT->GetKey(VK_LEFT) && player[1].rc.left >
		(field.left + (field.right - field.left) / 2)) {
		player[1].speed += power;

		if (player[1].speed > speedLimit)
			player[1].speed = speedLimit;

		player[1].x -= player[1].speed;
		player[1].rc = RectMakeCenter(
			player[1].x, player[1].y, player[1].width, player[1].height);
	}

	if (INPUT->GetKey(VK_RIGHT) && player[1].rc.right < field.right) {
		player[1].speed += power;

		if (player[1].speed > speedLimit)
			player[1].speed = speedLimit;

		player[1].x += player[1].speed;
		player[1].rc = RectMakeCenter(
			player[1].x, player[1].y, player[1].width, player[1].height);
	}

	if (INPUT->GetKey(VK_UP) && player[0].rc.top > field.top) {
		player[1].speed += power;

		if (player[1].speed > speedLimit)
			player[1].speed = speedLimit;

		player[1].y -= player[1].speed;
		player[1].rc = RectMakeCenter(
			player[1].x, player[1].y, player[1].width, player[1].height);
	}

	if (INPUT->GetKey(VK_DOWN) && player[1].rc.bottom < field.bottom) {
		player[1].speed += power;

		if (player[1].speed > speedLimit)
			player[1].speed = speedLimit;

		player[1].y += player[1].speed;
		player[1].rc = RectMakeCenter(
			player[1].x, player[1].y, player[1].width, player[1].height);
	}

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {

		ball.speed -= 0.05f;

		if (ball.speed <= 0) {
			ball.speed = 0;
			isMove = false;
		}

		if (circleCollision(player[0].rc, ball.rc) == true) {
			ball.speed += player[0].speed;
			player[0].speed = 0;
			angle = GetAngle(player[0].rc, ball.rc);
			isMove = true;
		}

		if (circleCollision(player[1].rc, ball.rc) == true) {
			ball.speed += player[1].speed;
			player[1].speed = 0;
			angle = GetAngle(player[1].rc, ball.rc);
			isMove = true;
		}

		if (isMove) {
			ball.x += cos(angle) * ball.speed;
			ball.y += -sin(angle) * ball.speed;

			ball.rc = RectMakeCenter(
				ball.x, ball.y, ball.width, ball.height);

			RECT temp;
			if (IntersectRect(&temp, &ball.rc, &score[0].rc)) {
				score[1].score += 1;
				TagInit();
			}

			if (IntersectRect(&temp, &ball.rc, &score[1].rc)) {
				score[0].score += 1;
				TagInit();
			}

			if (borderCollision()) {
				if (ball.rc.left < field.left) {
					angle = PI - angle;
				}
				else if (ball.rc.top < field.top) {
					angle = 2 * PI - angle;
				}
				else if (ball.rc.right > field.right) {
					angle = PI - angle;
				}
				else if (ball.rc.bottom > field.bottom) {
					angle = 2 * PI - angle;
				}
			}
		}
	}
}

void MainGame8::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	RectangleMake(memDC, field);

	LineMake(memDC, WINSIZEX / 2, WINSIZEY / 2 - 300, 
		WINSIZEX / 2, WINSIZEY / 2 + 300);

	char str[128];

	sprintf_s(str, "플레이어1 속도 : %f", player[0].speed);
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "플레이어2 속도 : %f", player[1].speed);
	TextOut(memDC, 10, 30, str, strlen(str));

	sprintf_s(str, "공 속도 : %f", ball.speed);
	TextOut(memDC, 10, 50, str, strlen(str));

	HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

	sprintf_s(str, "%d", score[0].score);
	TextOut(memDC, WINSIZEX / 2 - 200, WINSIZEY / 2 - 50, str, strlen(str));

	sprintf_s(str, "%d", score[1].score);
	TextOut(memDC, WINSIZEX / 2 + 150, WINSIZEY / 2 - 50, str, strlen(str));

	SelectObject(memDC, OldFont);
	DeleteObject(hFont);

	for (int i = 0; i < 2; i++) {
		EllipseMake(memDC, player[i].rc);
		RectangleMake(memDC, score[i].rc);
	}

	// 이렇게 하면 원도 색칠 가능
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);
	
	EllipseMake(memDC, ball.rc);

	SelectObject(memDC, oldBrush);
	DeleteObject(myBrush);



	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

bool MainGame8::circleCollision(RECT rc1, RECT rc2)
{
	POINT center1;
	POINT center2;

	RECT rc[2];
	rc[0] = rc1;
	rc[1] = rc2;

	center1.x = rc[0].left + (rc[0].right - rc[0].left) / 2;
	center1.y = rc[0].top + (rc[0].bottom - rc[0].top) / 2;

	center2.x = rc[1].left + (rc[1].right - rc[1].left) / 2;
	center2.y = rc[1].top + (rc[1].bottom - rc[1].top) / 2;

	int deltaX = center2.x - center1.x;
	int deltaY = center2.y - center1.y;

	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (player[0].width/2 + player[1].width/2 > distance) {
		return true;
	}
	return false;
}

bool MainGame8::borderCollision()
{

	for (int i = 0; i < 4; i++) {
		switch (i) {
			// left collision
		case 0:
			if (ball.rc.left < field.left)
				return true;
			break;
			// top collision
		case 1:
			if (ball.rc.top < field.top)
				return true;
			break;
			// right collsion
		case 2:
			if (ball.rc.right > field.right)
				return true;
			break;
			// bottom collsion
		case 3:
			if (ball.rc.bottom > field.bottom)
				return true;
			break;
		}
	}

	return false;
}

float MainGame8::GetAngle(RECT rc1, RECT rc2)
{
	POINT center1;
	POINT center2;

	RECT rc[2];
	rc[0] = rc1;
	rc[1] = rc2;

	center1.x = rc[0].left + (rc[0].right - rc[0].left) / 2;
	center1.y = rc[0].top + (rc[0].bottom - rc[0].top) / 2;

	center2.x = rc[1].left + (rc[1].right - rc[1].left) / 2;
	center2.y = rc[1].top + (rc[1].bottom - rc[1].top) / 2;

	int deltaX = center2.x - center1.x;
	int deltaY = center2.y - center1.y;

	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	float angle = acos(deltaX / distance);
	
	if (center2.y > center1.y) {
		angle = -angle;
	}

	return angle;
}
