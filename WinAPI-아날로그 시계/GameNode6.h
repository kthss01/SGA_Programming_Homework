#pragma once

#include "Image2.h"

class GameNode6
{
private:
	Image2 * m_backBuffer;
	void SetBackBuffer();
public:
	GameNode6();
	~GameNode6();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	Image2* GetBackBuffer() { return m_backBuffer; }

	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

