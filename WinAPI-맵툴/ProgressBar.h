#pragma once

#include "GameNode.h"

// 사용방법

class ProgressBar : public GameNode
{
private:
	RECT _rcProgressBar;
	float _x, _y;
	int _width;

	Image* _progressbarFront;
	Image* _progressbarBack;

public:
	ProgressBar();
	~ProgressBar();

	HRESULT Init(char* frontImage, char* backImage,
		float x, float y, float width, float height);
	void Release();
	void Update();
	void Render();

	void SetGauge(float currentGauge, float maxGauge);

	void SetPosition(int x, int y) { _x = x; _y = y; }
};

