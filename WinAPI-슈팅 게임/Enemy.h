#pragma once

#include "GameNode.h"
#include "Bullet.h"

struct tagEnemy {
	RECT rc;
	float x, y;
	float width, height;
	float speed;
	float angle;
	float moveFrame;
};

class Enemy : public GameNode
{
private:
	vector<tagEnemy> _vEnemy;
	vector<tagEnemy>::iterator _viEnemy;

	Image * _enemyImage;

	Bullet * _bullet;

	bool _isLeft;
	bool _isDown;
	bool _isInit;

	int _delay;
public:
	Enemy();
	~Enemy();

	HRESULT Init(int enemyMax);
	void Release() override;
	void Update() override;
	void Render() override;

	void Move();
	void Fire(float destX, float destY);
};

