#pragma once

#include "Enemy.h"

class Alien : public Enemy
{
private:
	Image * _died;

	int _frameCount;
	int _currentFrameX2;
public:
	Alien();
	~Alien();

	HRESULT Init(const char* imageName, POINT position) override;
	void Release() override;
	void Update() override;
	void Render() override;

	void Draw() override;
	void Animation() override;
};