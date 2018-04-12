#pragma once

#include "SIngletonBase.h"

#include "TileMap.h"

enum CTRL
{
	CTRL_TERRAINDRAW,
	CTRL_OBJECTDRAW,
	CTRL_ERASER,
	CTRL_SAVE,
	CTRL_LOAD,
	CTRL_END
};

class MainGame;

class Window : public SingletonBase<Window>
{
private:
	HWND hWnd;
	static POINT ptMouse;

	Image * m_backBuffer;
	GameNode * currentScene;

	RECT rc[8][20];
	POINT clickFrame;

	// 버튼을 만들어서 사용할 녀석
	HWND _btnTerrainDraw;
	HWND _btnObjectDraw;
	HWND _btnEraser;
	
	HWND _btnSave;
	HWND _btnLoad;

	static CTRL _currentCTRL;
	bool isActive;

	MainGame * mg;
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

	POINT GetFramePoint() { return clickFrame; }
	static POINT GetMousePos() { return ptMouse; }
	static CTRL GetCTRL() { return _currentCTRL; }

	void SetIsActive(bool isActive) { this->isActive = isActive; }
	bool GetIsActive() { return isActive; }

	void SetMainGame(MainGame * mg) { this->mg = mg; }
	MainGame * GetMainGame() { return mg; }

	void CreateSubWindow();
};

#define SUBWIN Window::GetSingleton()