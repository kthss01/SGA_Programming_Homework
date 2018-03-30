#pragma once

#include "GameNode.h"
#include "Bullet.h"

#include "Alien.h"
#include "Rocket.h"

class EnemyManager : public GameNode
{
private:
	typedef vector<Enemy*> vEnemy;
	typedef vector<Enemy*>::iterator viEnemy;
private:
	vEnemy _vAlien;
	viEnemy _viAlien;
	Bullet* _bullet;

	Rocket* _rocket;

	bool _isLeft;
	bool _isDown;
	bool _isInit;

	int _delay;
public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	// 동적할당 후 
	void SetAlien();
	void AlienBulletFire();
	void AlienMove();

	void SetRocket(Rocket* rocket) { _rocket = rocket; }
};