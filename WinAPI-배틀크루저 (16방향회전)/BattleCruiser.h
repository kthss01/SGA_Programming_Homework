#pragma once

#include "Ship.h"

class BattleCruiser : public Ship
{
private:
	bool _isMoving;
	float _endX, _endY;
	float _worldTimeCount;

public:
	BattleCruiser();
	~BattleCruiser();

	virtual HRESULT Init(const char* imageName, int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void MoveTo(float endX, float endY, float startX, float startY);
};

