#pragma once

#include "GameNode2.h"

class MainGame3 : public GameNode2
{
private:
	RECT rc[16];

	int score;
	int number;
	int time;
	int level;
	int maxLevel;
	int minTime;
	int count;

	HBRUSH brush;
	HBRUSH clickedBrush;

	bool isClicked;

	float prevTime;
	float currentTime;

public:
	MainGame3();
	~MainGame3();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

