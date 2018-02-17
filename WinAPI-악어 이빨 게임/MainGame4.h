#pragma once

#include "GameNode3.h"

class MainGame4 : public GameNode3
{
private:
	RECT rc[10];
	bool isClicked[10];

	int number;
	int score;

	HBRUSH brush;
	HBRUSH gameoverBrush;

	bool isOver;

	float prevTime;
	float currentTime;

public:
	MainGame4();
	~MainGame4();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	HRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

