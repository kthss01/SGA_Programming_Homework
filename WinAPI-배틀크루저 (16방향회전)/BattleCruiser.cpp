#include "stdafx.h"
#include "BattleCruiser.h"


BattleCruiser::BattleCruiser()
{
}


BattleCruiser::~BattleCruiser()
{
}

HRESULT BattleCruiser::Init(const char * imageName, int x, int y)
{
	Ship::Init(imageName, x, y);

	return S_OK;
}

void BattleCruiser::Release()
{
}

void BattleCruiser::Update()
{
	Ship::Update();
}

void BattleCruiser::Render()
{
	Ship::Render();
}

void BattleCruiser::MoveTo(float endX, float endY, float startX, float startY)
{
	if (!_isMoving) {

	}
}
