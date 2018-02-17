#include "stdafx.h"
#include "MainGame4.h"


MainGame4::MainGame4()
{
}


MainGame4::~MainGame4()
{
}

HRESULT MainGame4::Init()
{
	GameNode3::Init();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++)
			rc[i * 5 + j] = RectMakeCenter(j * 150 + 200, i * 150 + 400, 100, 100);
	}

	for (int i = 0; i < 10; i++)
		isClicked[i] = false;

	number = RND->GetInt(10);
	score = 0;

	brush = CreateSolidBrush(RGB(0, 255, 0));
	gameoverBrush = CreateSolidBrush(RGB(255, 0, 0));

	isOver = false;

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame4::Release()
{
	GameNode3::Release();
}

void MainGame4::Update()
{
	GameNode3::Update();

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {

		prevTime = GetTickCount();
	}

	if (isOver) {
		this->Release();
	}
}

void MainGame4::Render(HDC hdc)
{
	GameNode3::Render(hdc);
	
	char str[128];

	sprintf_s(str, "X: %d Y: %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hdc, 0, 0, str, strlen(str));

	for (int i = 0; i < 10; i++) {
		if (isClicked[i]) {
			// test
			sprintf_s(str, "%d 번째 클릭됨", i);
			TextOut(hdc, 0, 50, str, strlen(str));

			if (number == i) {
				FillRect(hdc, &rc[i], gameoverBrush);
				isOver = true;
			}
			else
				FillRect(hdc, &rc[i], brush);
		}
		else
			RectangleMake(hdc, rc[i]);
	}
	sprintf_s(str, "점수 : %d", score);
	TextOut(hdc, 150, 150, str, strlen(str));

	sprintf_s(str, "힌트 : %d", number);
	TextOut(hdc, 150, 200, str, strlen(str));

	if (isOver) {
		TextOut(hdc, WINSIZEX/2 - 50, WINSIZEY/2 - 200, "게임 오버", strlen("게임 오버"));
	}
}

HRESULT MainGame4::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if(!isOver) {
			for (int i = 0; i < 10; i++) {
				if (PtInRect(&rc[i], g_ptMouse) && isClicked[i] == false) {
					isClicked[i] = true;
					score += 1000;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				}
			}
		}

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
