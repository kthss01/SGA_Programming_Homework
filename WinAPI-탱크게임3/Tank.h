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

	tagTile* _targetTile;
	
	vector<tagTile*> _openList;
	vector<tagTile*> _closeList;

	int _lastIndex;
	int _Cx, _Cy, _Cg;

	int _startX, _startY;
	int _endX, _endY;
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

	void PathInit();
	void PathFind();
	POINT GetPos() { return PointMake((int)(_x / TILESIZE), (int)(_y / TILESIZE)); }
};

