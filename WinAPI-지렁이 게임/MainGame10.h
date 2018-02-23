#pragma once

#include "GameNode.h"

struct tagHeadInfo {
	RECT eye;
	RECT rc;
	int x, y;
	int radius;
};

struct tagTailInfo {
	RECT rc;
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

