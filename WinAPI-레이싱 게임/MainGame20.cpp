#include "stdafx.h"
#include "MainGame20.h"


MainGame20::MainGame20()
{
}


MainGame20::~MainGame20()
{
}

HRESULT MainGame20::Init()
{
	GameNode::Init();

	isDebug = false;
	isOver = false;

	_bg = new Image;
	_bg->Init("images/background.bmp", WINSIZEX, WINSIZEY);

	_speedometer = new Image;
	_speedometer->Init("images/speedometer.bmp", 
		150, 150, true, RGB(182, 185, 183));

	int tempY = 6;

	_character[0] = new Image;
	_character[0]->Init("images/wario.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[1] = new Image;
	_character[1]->Init("images/bowser.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[2] = new Image;
	_character[2]->Init("images/donkeykong.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[3] = new Image;
	_character[3]->Init("images/luigi.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[4] = new Image;
	_character[4]->Init("images/mario.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[5] = new Image;
	_character[5]->Init("images/peach.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[6] = new Image;
	_character[6]->Init("images/toad.bmp", 
		WINSIZEX / 9, WINSIZEY / tempY, true, RGB(182, 185, 183));
	_character[7] = new Image;
	_character[7]->Init("images/yoshi.bmp", 
		WINSIZEX / 9, WINSIZEY / 8, true, RGB(182, 185, 183));

	for (int i = 0; i < 8; i++) {
		if (i == 0) {
			_character[i]->SetFrameX(6);
			_character[i]->SetFrameY(1);
			_character[i]->SetX(WINSIZEX / 2 + _character[i]->GetWidth() / 2);
			_character[i]->SetY(WINSIZEY - _character[i]->GetHeight() / 2);
		}
		else {
			_character[i]->SetFrameX(0);
			_character[i]->SetFrameY(1);
			_character[i]->SetX(_character[i]->GetWidth() / 2 
				+ _character[i]->GetWidth() * i);
			_character[i]->SetY(-_character[i]->GetHeight() / 2
			- RND->GetInt(1000));
		}
	}

	offsetX = 0;
	offsetY = 0;

	speed = 0;
	maxSpeed = 20.0f;

	length = 50;
	ptX = WINSIZEX - _speedometer->GetWidth() / 2
		+ cosf(-speed / maxSpeed * PI - PI * 3 / 4) * length;
	ptY = WINSIZEY - _speedometer->GetHeight() / 2
		+ -sinf(-speed / maxSpeed * PI - PI * 3 / 4) * length;

	return S_OK;
}

void MainGame20::Release()
{
	GameNode::Release();
	SAFE_DELETE(_bg);
	for (int i = 0; i < 8; i++)
		SAFE_DELETE(_character[i]);
}

void MainGame20::Update()
{
	GameNode::Update();

	if (INPUT->GetKey(VK_UP)) { 
		if(speed < maxSpeed)
			speed += 0.25f; 
	}
	//if (INPUT->GetKey(VK_DOWN)) { 
	//	if(speed > -maxSpeed / 4)
	//	speed -= 0.25f; 
	//}
	if (INPUT->GetKey(VK_LEFT) &&
		_character[0]->GetX() > _character[0]->GetWidth() +
		_character[0]->GetWidth() / 2) { 
		_character[0]->SetX(_character[0]->GetX() - 5.0f); 
	}
	if (INPUT->GetKey(VK_RIGHT) && 
		_character[0]->GetX() < WINSIZEX - _character[0]->GetWidth() / 2
		- _character[0]->GetWidth() / 4
		- _character[0]->GetWidth()) {
		_character[0]->SetX(_character[0]->GetX() + 5.0f);
	}

	offsetY -= speed;

	for (int i = 1; i < 8; i++) {
		_character[i]->SetY(_character[i]->GetY() + speed);

		if (_character[i]->GetY()
			- _character[i]->GetHeight() / 2 > WINSIZEY)
			_character[i]->SetY(-_character[i]->GetHeight() / 2);

		if (i != 1 && IntersectRect(&temp,
			//&_character[0]->GetBoundingBox(),
			&RectMakeCenter(_character[0]->GetX(), _character[0]->GetY(),
				_character[0]->GetWidth() / 2,
				_character[0]->GetHeight() / 2),
			//&_character[i]->GetBoundingBox())) {
			&RectMakeCenter(_character[i]->GetX(), _character[i]->GetY(),
				_character[i]->GetWidth() / 2,
				_character[i]->GetHeight() / 2))) {
			speed -= 10;
			if (speed <= 0) {
				KillTimer(g_hWnd, 1);
				isOver = true;
			}
			_character[i]->SetY(-_character[i]->GetFrameHeight() / 2
				- RND->GetInt(1000));
		}
	}

	if (speed > 0)
		speed -= 0.125f;
	else if (speed < 0)
		speed += 0.125f;

	ptX = WINSIZEX - _speedometer->GetWidth() / 2
		+ cosf(-speed / maxSpeed * PI - PI * 3 / 4) * length;
	ptY = WINSIZEY - _speedometer->GetHeight() / 2
		+ -sinf(-speed / maxSpeed * PI - PI * 3 / 4) * length;

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame20::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		//_bg->Render(memDC);
		//_character[0]->Render(memDC);
		//_character[0]->FrameRender(memDC, 
		//	_character[0]->GetX(), 
		//	_character[0]->GetY());

		_bg->LoopRender(memDC, 
			&RectMake(0, 0, WINSIZEX, WINSIZEY),offsetX, offsetY);

		for (int i = 0; i < 8; i++) {
			//_character[i]->SetTransColor(false, RGB(255, 0, 255));
			_character[i]->Render(memDC,
				_character[i]->GetX() - _character[i]->GetWidth() / 2,
				_character[i]->GetY() - _character[i]->GetHeight() / 2);
		}

		_speedometer->Render(memDC, 
			WINSIZEX - _speedometer->GetWidth(),
			WINSIZEY - _speedometer->GetHeight());

		HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(memDC, pen);

		LineMake(memDC, 
			WINSIZEX - _speedometer->GetWidth() / 2, 
			WINSIZEY - _speedometer->GetHeight() / 2,
			ptX, ptY);

		SelectObject(memDC, oldPen);
		DeleteObject(pen);

		if (isOver) {
			SetBkMode(memDC, TRANSPARENT);
			//SetTextColor(memDC, RGB(255, 255, 255));

			HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
			HFONT oldFont = (HFONT)SelectObject(memDC, hFont);

			sprintf_s(str, "Game Over");
			TextOut(memDC, WINSIZEX / 2 - 100, WINSIZEY / 2 - 100, str, strlen(str));

			SelectObject(memDC, oldFont);
			DeleteObject(hFont);
		}
		
	}
	//=================================================
	//==================   Debug   ====================
	if (isDebug)
	{
		for (int i = 0; i < 8; i++) {
			//RectangleMake(memDC, _character[i]->GetBoundingBox());
			RectangleMake(memDC,
				RectMakeCenter(_character[i]->GetX(), _character[i]->GetY(),
					_character[i]->GetWidth() / 2,
					_character[i]->GetHeight() / 2));
		}

		sprintf_s(str, "speed : %f", speed);
		TextOut(memDC, WINSIZEX - 150, WINSIZEY - 175, str, strlen(str));
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
