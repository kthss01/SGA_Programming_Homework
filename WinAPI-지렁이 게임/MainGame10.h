#pragma once

#include "GameNode.h"

struct tagEyeInfo {
	float x, y;
	int radius;
};

struct tagHeadInfo {
	tagEyeInfo eye;
	float x, y;
	int radius;
};

struct tagTailInfo {
	int x, y;
	int radius;
};

class MainGame10 : public GameNode
{
#define MAX_WORM 50

private:
	
	tagHeadInfo head;
	tagTailInfo tail[MAX_WORM];

	float angleSpeed;
	float speed;
	float angle;

	int tailCount;

	float prevTime;
	float currentTime;

	bool isStart;
public:
	MainGame10();
	~MainGame10();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void WormMove();
	void WormTail();
};

