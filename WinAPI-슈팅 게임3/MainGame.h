#pragma once

#include "GameNode.h"
#include "Rocket.h"
#include "EnemyManager.h"

#include "ProgressBar.h"

#define LOADMAX 100

class MainGame : public GameNode
{
private:
	bool isDebug;
	bool isStart;
	bool isLoad;

	EnemyManager * _em;
	Rocket * _rocket;

	ProgressBar * _loading;

	int loadCount;

	float offsetY;
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};
