#pragma once

#include "GameNode.h"

class MainGame20 : public GameNode
{
private:
	Image * _bg;
	Image * _character[8];
	Image * _speedometer;

	int offsetX;
	int offsetY;

	float ptX;
	float ptY;
	float length;

	float maxSpeed;
	float speed;

	char str[128];
	RECT temp;

	bool isOver;
	bool isDebug;
public:
	MainGame20();
	~MainGame20();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

