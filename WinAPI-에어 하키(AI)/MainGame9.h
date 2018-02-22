#pragma once

#include "GameNode.h"

struct tagCircleInfo {
	RECT rc;
	int x, y;
	float speed;
	int width, height;
};

struct tagScoreInfo {
	RECT rc;
	int x, y;
	int score;
};

class MainGame9 : public GameNode
{
private:
	RECT field;

	tagCircleInfo player[2];
	tagCircleInfo ball;

	tagScoreInfo score[2];

	float power;

	float currentTime;
	float prevTime;

	float angle;

	bool isMove;
	bool isStart;

	int speedLimit;

	POINT old_ptMouse;

public:
	MainGame9();
	~MainGame9();

	void TagInit();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	bool circleCollision(RECT rc1, RECT rc2);
	bool borderCollision();
	float GetAngle(RECT rc1, RECT rc2);

	float GetDistance(RECT rc1, RECT rc2);
};

