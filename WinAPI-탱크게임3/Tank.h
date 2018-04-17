#pragma once

#include "GameNode.h"
#include "TankMap.h"

class Enemy;

class Tank : public GameNode
{
private:
	TANKDIRECTION _direction;
	Image * _image;
	RECT _rc;
	float _x, _y;
	float _speed;

	TankMap * _tankMap;

	Ray ray;

	vector<int> vTileIndex;

	int frameX, frameY;

	Enemy * enemy;

	bool isLive;
public:
	Tank();
	~Tank();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void TankMove();
	void TankFire();
	void SetTankPosition();
	void SetTankMapMemoryLink(TankMap * tm) { _tankMap = tm; }
	void SetEnemyMemoryLink(Enemy* enemy) { this->enemy = enemy; }
	RECT GetRect() { return _rc; }
	void SetIsLive(bool isLive) { this->isLive = isLive; }
	bool GetIsLive() { return isLive; }
};

