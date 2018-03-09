#include "stdafx.h"
#include "MainGame21.h"


MainGame21::MainGame21()
{
}


MainGame21::~MainGame21()
{
}

HRESULT MainGame21::Init()
{
	GameNode::Init();

	_bg = new Image;
	_bg->Init("images/stair.bmp", WINSIZEX, WINSIZEY);

	_ball = new Image;
	// 크기가 작은 공
	_ball->Init("images/ball.bmp", 60, 60, true, RGB(255, 0, 255));
	// 크기가 큰 공
	//_ball->Init("images/ball.bmp", 100, 100, true, RGB(255, 0, 255));

	// 볼의 시작 위치
	_x = 30;
	_y = WINSIZEY - 175;

	// 그리기 편하게 하기 위해 볼에 대한 Rect값 구하기
	_rc = RectMakeCenter(_x, _y, _ball->GetWidth(), _ball->GetHeight());

	// 볼에 밑바닥에서 탐지
	_probeY = _y + _ball->GetHeight() / 2;
	// 볼에 우측 끝에서 탐지
	_probeX = _x + _ball->GetWidth() / 2;

	isDebug = false;
	// 움직이는지 확인 하기 위한 변수
	isMove = false;

	return S_OK;
}

void MainGame21::Release()
{
	GameNode::Release();

	SAFE_DELETE(_bg);
	SAFE_DELETE(_ball);
}

void MainGame21::Update()
{
	GameNode::Update();

	// 방향키에 따라 움직이고 움직일때 bool 형 변수 true 
	if (INPUT->GetKey(VK_LEFT)) { _x -= 2.0f; isMove = true; }
	// 움직이지 않으면 false
	if (INPUT->GetKeyUp(VK_LEFT)) { isMove = false; }
	if (INPUT->GetKey(VK_RIGHT)) { _x += 2.0f; isMove = true; }
	if (INPUT->GetKeyUp(VK_RIGHT)) { isMove = false; }

	// Y의 탐지 범위 (볼에 바닥) 최신화
	_probeY = _y + _ball->GetHeight() / 2;

	// 떨어져야 하는지 확인하는 변수
	bool isFall = true;

	// 밑바닥 - 10 (위) 부터 밑바닥 + 10까지 탐지
	for (int i = _probeY - 10; i < _probeY + 10; i++) {
		// 해당 위치에 픽셀 값 읽어옴
		COLORREF color = GetPixel(_bg->GetMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		
		// 해당 픽셀이면 (색깔이 있는 부분이면)
		if (r == 185 && g == 122 && b == 86) {
			// 볼의 y값을 탐지한 위치에서 볼의 높이의 반만큼 올림
			// 볼의 높이의 반만큼 올리면 볼의 y 중점
			_y = i - _ball->GetHeight() / 2;
			// 떨어지지 않아야 하므로 false
			isFall = false;
			break;
		}
	}

	// y값에 대해 탐지를 했는데 탐지 되지 않았으면 isFall true 이므로 10만큼 떨어짐
	if (isFall) {
		_y += 10;
	}

	// 볼의 X의 탐지 범위 최신화 (볼의 오른쪽 끝으로 탐지)
	_probeX = _x + _ball->GetWidth() / 2;

	// 탐지범위 - 10 부터 X 의 탐지 전까지
	for (int i = _probeX - 10; i < _probeX; i++) {
		COLORREF color = GetPixel(_bg->GetMemDC(), i, _y - _ball->GetWidth() / 2);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		// 해당 픽셀이면
		if (r == 185 && g == 122 && b == 86) {
			// 볼의 중점 x를 탐지한 위치에서 너비/2로 함 (x 의 중점)
			_x = i - _ball->GetWidth() / 2;
			break;
		}
	}

	// 변경된 x, y 에 대해 rect 다시 만듬
	_rc = RectMakeCenter(_x, _y, _ball->GetWidth(), _ball->GetHeight());

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame21::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		_bg->Render(memDC);
		_ball->Render(memDC, _rc.left, _rc.top);

		char str[128];
		if (isMove) {
			sprintf_s(str, "움직이는 중");
		}
		else {
			sprintf_s(str, "정지 중");
		}
		TextOut(memDC, 10, 10, str, strlen(str));
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
