#pragma once

#include "GameNode.h"

struct tagBarrelnfo {
	float x, y;
	float size;
	float angle;
};

struct tagTankInfo {
	RECT head;
	float x, y;
	int radius;

	tagBarrelnfo barrel;
};

struct tagBulletInfo {
	float x, y;
	float radius;
	float speed;
	float angle;
	bool isFire;
	bool isBall;
	float shootTime;
};

#define BULLET_MAX 100

class MainGame11 : public GameNode
{
private:
	tagTankInfo tank;
	tagBulletInfo bullet[BULLET_MAX];

	float angleLimit;

	float prevTime;
	float currentTime;
public:
	MainGame11();
	~MainGame11();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

