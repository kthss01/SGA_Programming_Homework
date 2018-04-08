#pragma once

#include "SIngletonBase.h"

// 이미지 크기 + 알파 + (Y 경우) SAVE LOAD
// 16, 40은 좀더 주어야하는 값임 40은 위에 메뉴바이고 16은 뭔지 모르겟음
// Y에 미니맵 크기 추가 + 350
#define SUBWINSIZEX 21 * 20 + 16				// 436
#define SUBWINSIZEY 16 * 20 + 40 + 50 + 350		// 410 + 350 = 760

class Window : public SingletonBase<Window>
{
private:
	HWND hWnd;
	static POINT ptMouse;

	Image * m_backBuffer;
	GameNode * currentScene;

	bool isActive;
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

	POINT GetPtMouse() { return ptMouse; }

	void SetIsActive(bool isActive) { this->isActive = isActive; }
	bool GetIsActive() { return isActive; }
};

#define SUBWIN Window::GetSingleton()
