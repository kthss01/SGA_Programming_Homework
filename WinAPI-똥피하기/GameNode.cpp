#include "stdafx.h"
#include "GameNode.h"


GameNode::GameNode()
{
}


GameNode::~GameNode()
{
}

HRESULT GameNode::Init()
{
	SetTimer(g_hWnd, 1, 10, NULL);

	return S_OK;
}

void GameNode::Release()
{
	KillTimer(g_hWnd, 1);
	INPUT->ReleaseSingleton();
}

void GameNode::Update()
{
	//InvalidateRect(g_hWnd, NULL, TRUE);
	// 더블 버퍼링시
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void GameNode::Render(HDC hdc)
{
}

LRESULT GameNode::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT crt; // 직사각형 모양 범위 선언
	HDC hMemDC; // HDC를 하나 더 선언
	HBITMAP hBitmap, OldBitmap;	// HBITMAP은 대략 종이 의미, 종이 2장 선언

	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_TIMER:
		this->Update();
		break;
	case WM_PAINT:
		//// 기존 방법
		//hdc = BeginPaint(hWnd, &ps);
		//this->Render(hdc);
		//EndPaint(hWnd, &ps);

		// 더블 버퍼링 시도
		GetClientRect(hWnd, &crt);
		hdc = BeginPaint(hWnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);	// hMemDC에 기존 DC (hdc)에 맞게 새 DC 생성
		hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom); // crt 규격대로 종이 생성
		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); // 종이 교체

		FillRect(hMemDC, &crt, (HBRUSH)GetStockObject(WHITE_BRUSH)); // 흰색으로 화면 지우기
		this->Render(hMemDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY); // 배껴 그리기

		DeleteObject(SelectObject(hMemDC, OldBitmap)); // 종이 원래대로 한 후 제거
		DeleteDC(hMemDC); // hMemDC 제거

		EndPaint(hWnd, &ps);

		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
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
