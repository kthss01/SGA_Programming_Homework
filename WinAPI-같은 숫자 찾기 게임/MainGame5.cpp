#include "stdafx.h"
#include "MainGame5.h"


MainGame5::MainGame5()
{
}


MainGame5::~MainGame5()
{
}

void MainGame5::Shuffle()
{
	for (int i = 0; i < 1000; i++) {
		int dest = RND->GetInt(16);
		int sour = RND->GetInt(16);

		int temp = number[dest];
		number[dest] = number[sour];
		number[sour] = temp;
	}
}

HRESULT MainGame5::Init()
{
	GameNode4::Init();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			rc[i * 4 + j] = 
				RectMakeCenter(j * 150 + 150, i * 150 + 150, 100, 100);
		}
	}

	for (int i = 0; i < 16; i++) {
		number[i] = i % 8;
		isClicked[i] = false;
	}

	clickedNumber[0] = clickedNumber[1] = -1;
	count = 0;
	time = 0;

	isOver = false;
	isStart = false;

	Shuffle();

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame5::Release()
{
	GameNode4::Release();
}

void MainGame5::Update()
{
	GameNode4::Update();

	currentTime = GetTickCount();
	if (currentTime - prevTime > 500) {
		if(isStart)
			time++;

		isOver = true;
		for (int i = 0; i < 16; i++)
			if (isClicked[i] == false)
				isOver = false;

		if (count == 2) {
			if (number[clickedNumber[0]] == number[clickedNumber[1]])
			{
				
			}
			else {
				isClicked[clickedNumber[0]] = false;
				isClicked[clickedNumber[1]] = false;
			}
			clickedNumber[0] = clickedNumber[1] = -1;
			count = 0;
		}

		prevTime = GetTickCount();
	}
}

void MainGame5::Render(HDC hdc)
{
	GameNode4::Render(hdc);

	char str[128];

	for(int i=0; i<16; i++)
		RectangleMake(hdc, rc[i]);

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	OldFont = (HFONT)SelectObject(hdc, hFont);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (isClicked[i * 4 + j]) {
				sprintf_s(str, "%d", number[i * 4 + j]);
				TextOut(hdc, j * 150 + 140, i * 150 + 135, str, strlen(str));
			}
		}
	}

	sprintf_s(str, "걸린 시간 : %d 초", time/2);
	TextOut(hdc, 750, 350, str, strlen(str));

	if (!isStart) {
		TextOut(hdc, 250, 50, "시작하려면 마우스 왼쪽 버튼을 누르세요",
			strlen("시작하려면 마우스 왼쪽 버튼을 누르세요"));
	}

	if (isOver) {
		TextOut(hdc, 750, 300, "게임 클리어", strlen("게임 클리어"));
		this->Release();
	}

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	
}

LRESULT MainGame5::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT crt;
	HDC hMemDC;
	HBITMAP hBitmap, OldBitmap;

	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_TIMER:
		this->Update();
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		//this->Render(hdc);
		//EndPaint(hWnd, &ps);

		// 더블 버퍼링
		GetClientRect(hWnd, &crt);
		hdc = BeginPaint(hWnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		FillRect(hMemDC, &crt, (HBRUSH)GetStockObject(WHITE_BRUSH));
		this->Render(hMemDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(hMemDC, OldBitmap));
		DeleteDC(hMemDC);

		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);

		break;
	case WM_LBUTTONDOWN:
		if (!isStart)
			isStart = true;

		if (count < 2) {
			for (int i = 0; i < 16; i++) {
				if (PtInRect(&rc[i], g_ptMouse)) {
					isClicked[i] = true;
					clickedNumber[count++] = i;

					break;
				}
			}
		}

		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
