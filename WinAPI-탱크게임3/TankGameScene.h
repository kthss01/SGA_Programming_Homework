#pragma once

#include "GameNode.h"
#include "Tank.h"
#include "TankMap.h"
#include "Enemy.h"

class TankGameScene : public GameNode
{
private:
	Tank * tank;
	TankMap* map;
	Enemy * enemy;
public:
	TankGameScene();
	~TankGameScene();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
};

