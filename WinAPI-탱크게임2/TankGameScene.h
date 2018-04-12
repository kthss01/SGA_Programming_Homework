#pragma once

#include "GameNode.h"
#include "Tank.h"
#include "TankMap.h"

class TankGameScene : public GameNode
{
private:
	Tank * tank;
	TankMap* map;
public:
	TankGameScene();
	~TankGameScene();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
};

