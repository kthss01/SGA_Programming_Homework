#pragma once

#include "GameNode.h"

class StarcraftScene : public GameNode
{
private:
	class Ship * _battle;	// 전방선언과 동시에 변수를 만들 수 있음

	bool isDebug;
public:
	StarcraftScene();
	~StarcraftScene();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
};

