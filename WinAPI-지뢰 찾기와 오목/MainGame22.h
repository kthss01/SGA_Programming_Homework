#pragma once

#include "GameNode.h"

class MainGame22 : public GameNode
{
private:
	Image * bg;
	Image * icon1;
	Image * icon2;

	// 지뢰 아이콘 클릭을 위한 RECT
	RECT rcMineStart;
	// 오목 아이콘 클릭을 위한 RECT
	RECT rcGomokuStart;

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

