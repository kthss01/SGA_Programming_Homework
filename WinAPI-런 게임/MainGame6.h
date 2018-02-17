#pragma once

#include "GameNode5.h"

class MainGame6 : public GameNode5
{
private:
	RECT player;
	RECT rc[10];

	int score;

	HFONT hFont;
	HFONT OldFont;

	bool isJump;
	bool isStart;
	bool isOver;

	int gravity;
	int jumpPower;
	int jumpTime;

	float prevTime;
	float currentTime;

public:
	MainGame6();
	~MainGame6();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

