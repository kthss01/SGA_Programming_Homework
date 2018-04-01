#pragma once

#include "GameNode.h"
#include "Bullet.h"

#include "Alien.h"
#include "Boss.h"
#include "Rocket.h"

class ResultScene;

class EnemyManager : public GameNode
{
private:
	typedef vector<Enemy*> vEnemy;
	typedef vector<Enemy*>::iterator viEnemy;
private:
	vEnemy _vAlien;
	viEnemy _viAlien;
	Bullet* _bullet;

	vEnemy _vBoss;
	viEnemy _viBoss;
	Bullet* _bossBullet;

	Rocket* _rocket;

	bool _isLeft;
	bool _isDown;
	bool _isInit;
	bool _isFirst;

	bool _isBossFirst;
	bool _isBossLeft;

	int _delay;

	bool _cheat;

	ResultScene * _resultScene;
public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	// 동적할당 후 
	void SetAlien();
	void AddAlien();
	void AlienBulletFire();
	void AlienMove();

	void SetBoss();
	void BossBulletFire();
	void BossMove();

	void SetRocket(Rocket* rocket) { _rocket = rocket; }
	void SetResultScene(ResultScene * resultScene) { _resultScene = resultScene; }
};