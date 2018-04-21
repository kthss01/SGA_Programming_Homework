#pragma once

#include "SIngletonBase.h"

#define SUBWINSIZEX 250
#define SUBWINSIZEY 600

class Window : public SingletonBase<Window>
{
private:
	HWND hWnd;
	static POINT ptMouse;

	Image * m_backBuffer;
	GameNode * currentScene;

public:
	Window();
	~Window();

	void Init();
	void Release();
	void Update();
	void Render();
	void SetScene(GameNode* scene);
	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);
};

#define SUBWIN Window::GetSingleton()
