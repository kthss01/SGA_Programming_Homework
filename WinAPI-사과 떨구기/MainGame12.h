#pragma once

#include "GameNode.h"

struct tagGunInfo {
	float x, y;
	float size;
	float angle;
	float power;
};

struct tagTankInfo {
	RECT body;
	RECT head;
	tagGunInfo gun;
	float x, y;
	float width, height;
};

struct tagBulletInfo {
	float x, y;
	int radius;
	float speed;
	float shootTime;
	float gravityPower;
	float angle;
	bool isFire;
	bool isGravity;
};

struct tagAppleInfo {
	float x, y;
	int radius;
	float speed;
	bool isEnd;
	bool isGravity;
};

struct tagTreeInfo {
	RECT head;
	RECT body;
	int x, y;
	int radius;
};

#define BULLET_MAX 50
#define APPLE_MAX 10

class MainGame12 : public GameNode
{
private:
	tagTankInfo tank;
	tagBulletInfo bullet[BULLET_MAX];
	tagAppleInfo apple[APPLE_MAX];
	tagTreeInfo tree;

	RECT powerGauge;

	float angleLimit;
	float gravity;
	float power;

	float prevTime;
	float currentTime;
public:
	MainGame12();
	~MainGame12();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

