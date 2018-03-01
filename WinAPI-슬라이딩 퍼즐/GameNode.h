#pragma once

#include "Image.h"

class GameNode
{
private:
	Image * m_backBuffer;
	void SetBackBuffer();
public:
	GameNode();
	~GameNode();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	Image* GetBackBuffer() { return m_backBuffer; }

	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};