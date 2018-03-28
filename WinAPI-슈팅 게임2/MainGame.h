#pragma once

#include "GameNode.h"
#include "Rocket.h"
#include "EnemyManager.h"

class MainGame : public GameNode
{
private:
	bool isDebug;
	bool isStart;

	EnemyManager * _em;
	Rocket * _rocket;

	float offsetY;
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};
