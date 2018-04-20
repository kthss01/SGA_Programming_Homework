#pragma once

#include "SIngletonBase.h"

#define SUBWINSIZEX 240
#define SUBWINSIZEY 600

enum CTRL
{
	CTRL_TERRAINDRAW,
	CTRL_OBJECTDRAW,
	CTRL_ERASER,
	CTRL_SAVE,
	CTRL_LOAD,
	CTRL_END
};

class IsoMap;

class Window : public SingletonBase<Window>
{
private:
	HWND hWnd;
	static POINT ptMouse;

	Image * m_backBuffer;
	GameNode * currentScene;

	// 버튼을 만들어서 사용할 녀석
	HWND _btnTerrainDraw;
	HWND _btnObjectDraw;
	HWND _btnEraser;

	HWND _btnSave;
	HWND _btnLoad;

	static CTRL _currentCTRL;
	POINT clickFrame;

	bool isActive;

	IsoMap * map;
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

	HWND GethWnd() { return hWnd; }
	static POINT GetMousePos() { return ptMouse; }
	
	static CTRL GetCTRL() { return _currentCTRL; }
	void SetCTRL(CTRL ctrl) { _currentCTRL = ctrl; }

	void SetIsActive(bool isActive) { this->isActive = isActive; }
	bool GetIsActive() { return isActive; }

	void SetFramePoint(POINT frame) { clickFrame = frame; }
	POINT GetFramePoint() { return clickFrame; }

	void SetIsoMap(IsoMap * map) { this->map = map; }
	IsoMap * GetIsoMap() { return map; }

	void CreateSubWindow();
};

#define SUBWIN Window::GetSingleton()
