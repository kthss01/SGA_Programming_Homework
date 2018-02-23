#include "stdafx.h"
#include "MainGame12.h"


MainGame12::MainGame12()
{
}


MainGame12::~MainGame12()
{
}

HRESULT MainGame12::Init()
{
	GameNode::Init();
	
	// tank
	tank.x = 125;
	tank.y = WINSIZEY - 150;
	tank.width = 225;
	tank.height = 100;
	tank.body = RectMakeCenter(tank.x, tank.y + 50, tank.width, tank.height);
	tank.head = RectMakeCenter(tank.x, tank.y, 100, 100);

	tank.gun.size = 100;
	tank.gun.angle = 45 * M_PI / 180;
	tank.gun.x = tank.x + cosf(tank.gun.angle) * tank.gun.size;
	tank.gun.y = tank.y + -sinf(tank.gun.angle) * tank.gun.size;
	tank.gun.power = 0;

	// tree
	tree.x = WINSIZEX / 2 + WINSIZEX / 4;
	tree.y = WINSIZEY / 2 + WINSIZEY / 4;
	tree.radius = 200;
	tree.body = RectMakeCenter(tree.x, tree.y, 25, 300);
	tree.head = RectMakeCenter(
		tree.x, tree.y - 300, tree.radius * 2, tree.radius * 2);

	// apple
	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].isGravity = false;
		apple[i].isEnd = false;
		float randAngle = RND->GetInt(360);
		apple[i].x = tree.x + cosf(randAngle)
			* RND->GetFromInto(-tree.radius, tree.radius);
		apple[i].y = tree.y - 300 + -sinf(randAngle)
			* RND->GetFromInto(-tree.radius, tree.radius);
		apple[i].radius = 25;
		apple[i].speed = 0;
	}

	// bullet
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].isGravity = false;
		bullet[i].isFire = false;
		bullet[i].speed = 0;
		bullet[i].radius = 10;
	}

	// 파워 게이지
	powerGauge = RectMake(200, 10, 0, 25);

	angleLimit = 5.0f;
	gravity = 0.1f;
	power = 0;

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame12::Release()
{
	GameNode::Release();
}

void MainGame12::Update()
{
	GameNode::Update();

	if (INPUT->GetKey(VK_UP)) { tank.gun.angle += 0.025f; }
	if (INPUT->GetKey(VK_DOWN)) { tank.gun.angle -= 0.025f; }
	if (INPUT->GetKey(VK_LEFT)) { tank.x -= 3.0f; }
	if (INPUT->GetKey(VK_RIGHT)) { tank.x += 3.0f; }
	if (INPUT->GetKey(VK_SPACE)) { 
		power += 0.1f;
		powerGauge.right += 1.0f;
	}

	// 좌우 이동 제한
	if (tank.x - tank.width / 2 < 0)
		tank.x = tank.width / 2;
	if (tank.x + tank.width / 2 > WINSIZEX)
		tank.x = WINSIZEX - tank.width / 2;

	// 왼쪽 각 제한
	if (tank.gun.angle > M_PI - angleLimit * M_PI / 180)
		tank.gun.angle = M_PI - angleLimit * M_PI / 180;
	// 오른쪽 각 제한
	if (tank.gun.angle < angleLimit * M_PI / 180)
		tank.gun.angle = angleLimit * M_PI / 180;

	// 탱크 이동
	tank.body = RectMakeCenter(tank.x, tank.y + 50, tank.width, tank.height);
	tank.head = RectMakeCenter(tank.x, tank.y, 100, 100);

	// 포각 이동
	tank.gun.x = tank.x + cosf(tank.gun.angle) * tank.gun.size;
	tank.gun.y = tank.y + -sinf(tank.gun.angle) * tank.gun.size;

	// 총알 발사
	if (INPUT->GetKeyUp(VK_SPACE)) {
		powerGauge = RectMake(200, 10, 0, 25);
		for (int i = 0; i < BULLET_MAX; i++) {
			if (bullet[i].isFire == true) continue;

			bullet[i].isFire = true;
			bullet[i].angle = tank.gun.angle;
			bullet[i].x = tank.gun.x;
			bullet[i].y = tank.gun.y;
			bullet[i].speed = power;
			bullet[i].shootTime = GetTickCount();
			power = 0;
			bullet[i].gravityPower = 0.0f;
			break;
		}
	}

	// 필요 없음 총알마다 중력 필요
	//currentTime = GetTickCount();
	//if (currentTime - prevTime > 1000) {
	//	gravity++;
	//	prevTime = GetTickCount();
	//}

	currentTime = GetTickCount();

	// 총알 이동
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == false) continue;

		bullet[i].gravityPower += gravity;

		// 일정시간 지나면 중력 적용받게 하는거
		// 이거보다는 바로 중력 받는게 더 나아보임
		//if (currentTime - bullet[i].shootTime > 1500) {
		//	bullet[i].isGravity = true;
		//}

		//if (bullet[i].isGravity == true) {
		//	bullet[i].gravityPower += gravity;
		//	bullet[i].y += bullet[i].gravityPower;
		//}

		bullet[i].x += cosf(bullet[i].angle) * bullet[i].speed;
		//bullet[i].y += -sinf(bullet[i].angle) * bullet[i].speed;
		
		bullet[i].y += -sinf(bullet[i].angle) * bullet[i].speed + bullet[i].gravityPower;

		// 사과 충돌
		for (int j = 0; j < APPLE_MAX; j++) {
			if (apple[j].isGravity == true) continue;
			if (apple[j].isEnd == true) continue;

			if (GetDistance(bullet[i].x, bullet[i].y, apple[j].x, apple[j].y)
				< bullet[i].radius + apple[j].radius) {
				apple[j].isGravity = true;
			}
		}
	}

	// 사과 이동
	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].isGravity == false) continue;
		if (apple[i].isEnd == true) continue;
		apple[i].speed += gravity;
		apple[i].y += apple[i].speed;

		if (apple[i].y + apple[i].radius > WINSIZEY)
			apple[i].isEnd = true;
	}

	// 총알 재배치
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == false) continue;

		if (bullet[i].y + bullet[i].radius > WINSIZEY)
			bullet[i].isFire = false;
	}
}

void MainGame12::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	char str[128];
	
	int count = 0;
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == true) continue;
		count++;
	}

	HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

	sprintf_s(str, "파워 : %f", power);
	TextOut(memDC, 10, 10, str, strlen(str));
	sprintf_s(str, "포 갯수 : %d", count);
	TextOut(memDC, 10, 40, str, strlen(str));

	count = 0;
	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].isEnd == true) continue;
		count++;
	}

	sprintf_s(str, "사과 갯수 : %d", count);
	TextOut(memDC, 10, 70, str, strlen(str));

	SelectObject(memDC, OldFont);
	DeleteObject(hFont);

	RectangleMake(memDC, tree.body);
	EllipseMake(memDC, tree.head);

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].isEnd == true) continue;

		Ellipse(memDC,
			apple[i].x - apple[i].radius,
			apple[i].y - apple[i].radius,
			apple[i].x + apple[i].radius,
			apple[i].y + apple[i].radius);
	}

	RectangleMake(memDC, tank.body);
	LineMake(memDC, tank.x, tank.y, tank.gun.x, tank.gun.y);
	EllipseMake(memDC, tank.head);

	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFire == false) continue;

		Ellipse(memDC, 
			bullet[i].x - bullet[i].radius,
			bullet[i].y - bullet[i].radius,
			bullet[i].x + bullet[i].radius,
			bullet[i].y + bullet[i].radius);
	}

	if (count==0) {
		HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

		sprintf_s(str, "게임 클리어");
		TextOut(memDC, WINSIZEX / 8, WINSIZEY / 4, str, strlen(str));

		SelectObject(memDC, OldFont);
		DeleteObject(hFont);
	}

	RectangleMake(memDC, powerGauge);

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
