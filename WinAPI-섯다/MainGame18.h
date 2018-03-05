#pragma once

#include "GameNode.h"

#define BG_SIZE 25
#define COMBI_SIZE 14

class MainGame18 : public GameNode
{
private:
	Image * logo;
	Image * card;
	Image * bg[BG_SIZE];
	Image * combi[COMBI_SIZE];

	char str[128];

	float moveFrame = 0;
	bool isCombi = false;
public:
	MainGame18();
	~MainGame18();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

