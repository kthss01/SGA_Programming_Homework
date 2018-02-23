#include "stdafx.h"
#include "MainGame11.h"


MainGame11::MainGame11()
{
}


MainGame11::~MainGame11()
{
}

HRESULT MainGame11::Init()
{
	GameNode::Init();

	tank.x = WINSIZEX / 2;
	tank.y = WINSIZEY;
	tank.radius = 125;
	tank.head = RectMakeCenter(tank.x, tank.y, tank.radius, tank.radius);

	tank.barrel.angle = 90 * M_PI / 180;
	tank.barrel.size = 85.0f;
	tank.barrel.x = tank.x + cos(tank.barrel.angle) * 125.0f;
	tank.barrel.y = tank.y + -sin(tank.barrel.angle) * 125.0f;

	angleLimit = 10.0f;

	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].radius = 25;
		bullet[i].isFire = false;
		bullet[i].isBall = false;
	}

	return S_OK;
}

void MainGame11::Release()
{
	GameNode::Release();
}

void MainGame11::Update()
{
	GameNode::Update();

	// 탱크 포신 움직이기
	if (INPUT->GetKey(VK_LEFT)) { tank.barrel.angle += 2.0f * M_PI / 180; };
	if (INPUT->GetKey(VK_RIGHT)) { tank.barrel.angle -= 2.0f * M_PI / 180; };

	if (tank.barrel.angle > M_PI - angleLimit * M_PI / 180)
		tank.barrel.angle = M_PI - angleLimit * M_PI / 180;
	if (tank.barrel.angle < angleLimit * M_PI / 180)
		tank.barrel.angle = angleLimit * M_PI / 180;

	tank.barrel.x = tank.x + cosf(tank.barrel.angle) * tank.barrel.size;
	tank.barrel.y = tank.y + -sinf(tank.barrel.angle) * tank.barrel.size;

	// 총알 발사
	if (INPUT->GetKey(VK_SPACE)) {
		for (int i = 0; i < BULLET_MAX; i++) {
			if (bullet[i].isFire == true) continue;

			bullet[i].isFire = true;
			bullet[i].shootTime = GetTickCount();
			bullet[i].x = tank.barrel.x + cosf(tank.barrel.angle) * 15.0f;
			bullet[i].y = tank.barrel.y + -sinf(tank.barrel.angle) * 15.0f;
			bullet[i].angle = tank.barrel.angle;
			bullet[i].speed = 3.0f;
			break;
		}
	}

	currentTime = GetTickCount();

	// 총알 움직임
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == false) continue;

		bullet[i].x += cosf(bullet[i].angle) * bullet[i].speed;
		bullet[i].y += -sinf(bullet[i].angle) * bullet[i].speed;

		// 발사 후 일정시간 지나면 볼끼리 충돌되게
		if (currentTime - bullet[i].shootTime > 3000) {
			bullet[i].isBall = true;
		}

		// 총알 충돌
		// 벽 충돌
		if (bullet[i].x - bullet[i].radius < 0) {
			bullet[i].x = bullet[i].radius;
			bullet[i].angle = M_PI - bullet[i].angle;
		}
		if (bullet[i].x + bullet[i].radius > WINSIZEX) {
			bullet[i].x = WINSIZEX - bullet[i].radius;
			bullet[i].angle = M_PI - bullet[i].angle;
		}
		if (bullet[i].y - bullet[i].radius < 0) {
			bullet[i].y = bullet[i].radius;
			bullet[i].angle = 2 * M_PI - bullet[i].angle;
		}
		if (bullet[i].y + bullet[i].radius > WINSIZEY) {
			bullet[i].y = WINSIZEY - bullet[i].radius;
			bullet[i].angle = 2 * M_PI - bullet[i].angle;
		}

		// 일정시간 전 까지는 총알끼리 충돌 x
		if (bullet[i].isBall == false) continue;

		// 총알 끼리 충돌
		for (int j = 0; j < BULLET_MAX; j++) {
			if (bullet[j].isFire == false) continue;

			if (i == j) continue;

			float length = GetDistance
							(bullet[i].x, bullet[i].y, bullet[j].x, bullet[j].y);

			if (length > bullet[i].radius + bullet[j].radius) continue;
			
			bullet[i].angle = GetAngle(
				bullet[j].x, bullet[j].y, bullet[i].x, bullet[i].y);
			bullet[j].angle = GetAngle(
				bullet[i].x, bullet[i].y, bullet[j].x, bullet[j].y);
		}
	}
}

void MainGame11::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	HPEN pen = CreatePen(PS_SOLID, 30, RGB(0, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(memDC, pen);

	LineMake(memDC, tank.x, tank.y, tank.barrel.x, tank.barrel.y);

	SelectObject(memDC, oldPen);
	DeleteObject(pen);

	EllipseMake(memDC, tank.head);

	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == false) continue;
		//EllipseMake(memDC, bullet[i].rc);
		Ellipse(memDC, bullet[i].x - bullet[i].radius,
			bullet[i].y - bullet[i].radius,
			bullet[i].x + bullet[i].radius,
			bullet[i].y + bullet[i].radius);
	}

	char str[128];
	sprintf_s(str, "포 각도 : %f", tank.barrel.angle);
	TextOut(memDC, 10, 10, str, strlen(str));

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
