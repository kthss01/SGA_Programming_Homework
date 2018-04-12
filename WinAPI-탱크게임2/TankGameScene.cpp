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
	tank->SetTankMapMemorLink(map);
	tank->SetTankPosition();
	return S_OK;
}

void TankGameScene::Release()
{
	SAFE_DELETE(tank);
	SAFE_DELETE(map);
}

void TankGameScene::Update()
{
	tank->Update();
}

void TankGameScene::Render()
{
	map->Render();
	tank->Render();
}
