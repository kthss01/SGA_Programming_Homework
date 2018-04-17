#include "stdafx.h"
#include "TankGameScene.h"


TankGameScene::TankGameScene()
{
}


TankGameScene::~TankGameScene()
{
}

HRESULT TankGameScene::Init()
{
	tank = new Tank;
	tank->Init();
	map = new TankMap;
	map->Init();
	tank->SetTankMapMemoryLink(map);
	tank->SetTankPosition();
	enemy = new Enemy;
	enemy->Init();
	enemy->SetTankMapMemoryLink(map);
	enemy->SetTankPosition();
	tank->SetEnemyMemoryLink(enemy);
	enemy->SetTankMemoryLink(tank);
	return S_OK;
}

void TankGameScene::Release()
{
	SAFE_DELETE(tank);
	SAFE_DELETE(map);
	SAFE_DELETE(enemy);
}

void TankGameScene::Update()
{
	tank->Update();
	enemy->Update();
}

void TankGameScene::Render()
{
	map->Render();
	tank->Render();
	enemy->Render();
}
