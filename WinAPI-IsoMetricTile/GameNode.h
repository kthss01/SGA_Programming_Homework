#pragma once

#include "Image.h"

class GameNode
{
private:
	static Image* m_backBuffer;
	HDC _hdc;
public:
	GameNode();
	~GameNode();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
	virtual void Render(HDC hdc);

	HDC GetMemDC() { return m_backBuffer->GetMemDC(); }
	HDC GetHDC() { return _hdc; }
	static Image* SetBackBuffer();
	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};