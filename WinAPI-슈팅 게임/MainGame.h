#pragma once

#include "GameNode.h"

class MainGame : public GameNode
{
private:
	bool isDebug;

	bool isStart;

	float offsetY;
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};
