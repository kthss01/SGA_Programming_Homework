#pragma once

#include "GameNode.h"
#include "Bullet.h"

#include "Alien.h"
#include "Boss.h"
#include "Rocket.h"

// 결과씬 전방 선언
class ResultScene;

class EnemyManager : public GameNode
{
private:
	typedef vector<Enemy*> vEnemy;
	typedef vector<Enemy*>::iterator viEnemy;
private:
	// 에너미를 관리하는 vector
	vEnemy _vAlien;
	viEnemy _viAlien;
	// 에너미의 총알
	Bullet* _bullet;

	// 보스를 관리하는 vector
	vEnemy _vBoss;
	viEnemy _viBoss;
	// 보스의 총알
	Bullet* _bossBullet;

	Rocket* _rocket;

	// 에너미의 움직임에 필요한 변수들
	bool _isLeft;	
	bool _isDown;
	bool _isInit;
	// 에너미의 움직임이 처음인지 (처음이면 일정 위치까지 내려와야해서)
	bool _isFirst;

	// 보스의 움직임에 필요한 변수들
	// 보스의 움직임이 처음인지
	bool _isBossFirst;
	bool _isBossLeft;

	// 움직일 때 얼마동안 내려올지 정하기 위해 필요한 변수
	int _delay;

	// 테스트 편하게 하기 위한 치트
	bool _cheat;

	ResultScene * _resultScene;
public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	// 에너미 설정
	void SetAlien();
	// 에너미 추가 (보스 패턴 중 에너미 추가하는 패턴이 있어서)
	void AddAlien();
	// 에너미들의 총알 발사
	void AlienBulletFire();
	// 에너미들의 이동
	void AlienMove();

	// 보스 설정
	void SetBoss();
	// 보스의 총알 발사
	void BossBulletFire();
	// 보스의 이동
	void BossMove();

	void SetRocket(Rocket* rocket) { _rocket = rocket; }
	void SetResultScene(ResultScene * resultScene) { _resultScene = resultScene; }
};