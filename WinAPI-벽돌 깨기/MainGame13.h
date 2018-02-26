#pragma once

#include "GameNode.h"

#define BLOCKMAX 4 * 12

struct tagBlockInfo {
	RECT rc;
	float x, y;
	int life;
};

struct tagBallInfo {
	POINT pt;
	float radius;
	float angle;
	float speed;
};

struct tagBarInfo {
	RECT rc;
	float x, y;
};

class MainGame13 : public GameNode
{
private:
	tagBlockInfo block[BLOCKMAX];
	tagBallInfo ball;
	tagBarInfo bar;

	bool isStart;
	bool isOver;
public:
	MainGame13();
	~MainGame13();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	bool RectInCircle(RECT* rect, POINT circleCenter, float radius);
};

