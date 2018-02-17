#pragma once

#include "GameNode4.h"

class MainGame5 : public GameNode4
{
private:
	RECT rc[16];
	int number[16];
	
	int clickedNumber[2];
	int count;
	int time;

	bool isClicked[16];
	bool isOver;
	bool isStart;

	HFONT hFont;
	HFONT OldFont;

	float prevTime;
	float currentTime;

public:
	MainGame5();
	~MainGame5();

	void Shuffle();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

