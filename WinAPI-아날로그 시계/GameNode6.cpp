#include "stdafx.h"
#include "GameNode6.h"


GameNode6::GameNode6()
{
	SetBackBuffer();
}


GameNode6::~GameNode6()
{
}

void GameNode6::SetBackBuffer()
{
	m_backBuffer = new Image2;
	m_backBuffer->Init(WINSIZEX, WINSIZEY);
}

HRESULT GameNode6::Init()
{
	SetTimer(g_hWnd, 1, 10, NULL);

	return S_OK;
}

void GameNode6::Release()
{
	KillTimer(g_hWnd, 1);

	SAFE_DELETE(m_backBuffer);

	INPUT->ReleaseSingleton();
}

void GameNode6::Update()
{
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void GameNode6::Render(HDC hdc)
{
}

LRESULT GameNode6::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_TIMER:
		this->Update();
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		this->Render(hdc);
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
