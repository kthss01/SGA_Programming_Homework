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
	// 게임 끝났는지 확인 
	isOver = false;

	// 배경 이미지 읽어오기
	_bg = new Image;
	_bg->Init("images/background.bmp", WINSIZEX, WINSIZEY);

	// 계기판 이미지 읽어오기
	_speedometer = new Image;
	_speedometer->Init("images/speedometer.bmp", 
		150, 150, true, RGB(182, 185, 183));

	// 캐릭터 이미지 읽어오기
	// y값 일괄 설정을 위한 임시 변수
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

	// 각각의 캐릭터 이미지 위치 지정
	for (int i = 0; i < 8; i++) {
		// 플레이어 캐릭터
		if (i == 0) {
			_character[i]->SetX(WINSIZEX / 2 + _character[i]->GetWidth() / 2);
			_character[i]->SetY(WINSIZEY - _character[i]->GetHeight() / 2);
		}
		// 나머지 캐릭터
		else {
			_character[i]->SetX(_character[i]->GetWidth() / 2 
				+ _character[i]->GetWidth() * i);
			_character[i]->SetY(-_character[i]->GetHeight() / 2
			- RND->GetInt(1000));
		}
	}

	// 루프 렌더를 위한 x, y 변수
	offsetX = 0;
	offsetY = 0;

	// 속도 및 최고 속도
	speed = 0;
	maxSpeed = 20.0f;

	// 계기판 침 길이
	length = 50;
	// 계기판 침을 위한 x,y LineMake함수에서 쓰임
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

		// 바닥 내려가면 다시 위로
		if (_character[i]->GetY()
			- _character[i]->GetHeight() / 2 > WINSIZEY)
			_character[i]->SetY(-_character[i]->GetHeight() / 2);

		// 플레이어 캐릭터와 충돌 할때
		// 그냥 충돌하는게 너무 쉽게 되서 Rect 크기를 절반으로 줄여서 충돌 체크
		if (i != 1 && IntersectRect(&temp,
			//&_character[0]->GetBoundingBox(),
			&RectMakeCenter(_character[0]->GetX(), _character[0]->GetY(),
				_character[0]->GetWidth() / 2,
				_character[0]->GetHeight() / 2),
			//&_character[i]->GetBoundingBox())) {
			&RectMakeCenter(_character[i]->GetX(), _character[i]->GetY(),
				_character[i]->GetWidth() / 2,
				_character[i]->GetHeight() / 2))) {
			// 충돌 시 스피드 감소
			speed -= 10;
			// 스피드 0보다 작아지면 (정지 상태)
			if (speed <= 0) {
				// 타이머 죽여서 게임 멈추고
				KillTimer(g_hWnd, 1);
				// 게임 오버 bool 변수 true
				isOver = true;
			}
			// 충돌시 충돌한 캐릭터 (플레이어 캐릭터 말고) 화면 밖으로 보내기
			_character[i]->SetY(-_character[i]->GetFrameHeight() / 2
				- RND->GetInt(1000));
		}
	}

	// 일정시간 지나면 스피드 0으로 바뀌게
	if (speed > 0)
		speed -= 0.125f;
	else if (speed < 0)
		speed += 0.125f;

	// 계기판 속도에 맞춰서 변화하도록 
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

		// 루프렌더로 배경 움직임
		_bg->LoopRender(memDC, 
			&RectMake(0, 0, WINSIZEX, WINSIZEY),offsetX, offsetY);

		// 캐릭터 그리기
		for (int i = 0; i < 8; i++) {
			//_character[i]->SetTransColor(false, RGB(255, 0, 255));
			_character[i]->Render(memDC,
				_character[i]->GetX() - _character[i]->GetWidth() / 2,
				_character[i]->GetY() - _character[i]->GetHeight() / 2);
		}

		// 계기판 그리기
		_speedometer->Render(memDC, 
			WINSIZEX - _speedometer->GetWidth(),
			WINSIZEY - _speedometer->GetHeight());

		// 계기판 침 그리기
		HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(memDC, pen);

		LineMake(memDC, 
			WINSIZEX - _speedometer->GetWidth() / 2, 
			WINSIZEY - _speedometer->GetHeight() / 2,
			ptX, ptY);

		SelectObject(memDC, oldPen);
		DeleteObject(pen);

		// 게임 오버시
		if (isOver) {
			// 글자색 배경 투명
			SetBkMode(memDC, TRANSPARENT);
			//SetTextColor(memDC, RGB(255, 255, 255));

			// 글자색 크기 변경
			HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
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
