#pragma once

#include "GameNode.h"

class MainGame22 : public GameNode
{
private:
	Image * bg;
	Image * icon1;

	RECT rcMineStart;
	
	bool isGame;

	bool isDebug;
	char str[128];
public:
	MainGame22();
	~MainGame22();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};

