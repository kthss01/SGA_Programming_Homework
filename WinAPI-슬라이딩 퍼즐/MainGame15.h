#pragma once

#include "GameNode.h"

struct tagSlideInfo {
	float x, y;
	float offsetX, offsetY;
	float width, height;
};

class MainGame15 : public GameNode
{
private:
	Image * image;

	tagSlideInfo slide[9];
	int pointer;

public:
	MainGame15();
	~MainGame15();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void SlideInit();
	void SlideSwap(int sour, int dest);
};

