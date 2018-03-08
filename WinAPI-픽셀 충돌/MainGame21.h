#pragma once

#include "GameNode.h"

class MainGame21 : public GameNode
{
private:
	Image * _bg;
	Image * _ball;

	float _x, _y;
	float _probeY;
	float _probeX;

	RECT _rc;

	bool isMove;
	bool isDebug;
public:
	MainGame21();
	~MainGame21();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

