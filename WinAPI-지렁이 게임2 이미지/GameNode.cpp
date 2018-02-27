#include "stdafx.h"
#include "GameNode.h"

GameNode::GameNode() {
	SetBackBuffer();
}

GameNode::~GameNode() {}

void GameNode::SetBackBuffer() {
	m_backBuffer = new Image;
	m_backBuffer->Init(WINSIZEX, WINSIZEY);
}

HRESULT GameNode::Init() {
	SetTimer(g_hWnd, 1, 10, NULL);

	return S_OK;
}

void GameNode::Release() {
	KillTimer(g_hWnd, 1);

	SAFE_DELETE(m_backBuffer);

	INPUT->GetSingleton();
}

void GameNode::Update() {
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void GameNode::Render(HDC hdc) {}

LRESULT GameNode::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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