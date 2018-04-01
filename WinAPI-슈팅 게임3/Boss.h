#pragma once

#include "Enemy.h"
#include "ProgressBar.h"

enum BOSS_PARTS {
	BOSS_HATCH,
	BOSS_ENEMY,
	BOSS_DEPLOY,
	BOSS_FIRE,
	BOSS_END
};

class EnemyManager;

class Boss : public Enemy
{
private:
	Image * _images[BOSS_END];

	int _frameCount;
	int _currentFrameXs[BOSS_END];

	// 애니메이션 상태 확인을 위한 변수
	bool _isHatch;
	bool _isHatchClose;
	bool _isDeploy;
	bool _isDeployClose;
	bool _isFire;
	bool _isEnemy;

	// 현재 보스 패턴이 어떤 패턴인지 확인하기 위한 변수
	// 애니메이션 상태를 저장
	BOSS_PARTS currentParts;

	int _count;
	int _fireCount;

	int _hp;
	int _maxHp;
	ProgressBar * _hpBar;

	EnemyManager * _em;
public:
	Boss();
	~Boss();

	HRESULT Init(const char* imageName, POINT position) override;
	void Release() override;
	void Update() override;
	void Render() override;

	void Move(Direction dir) override;
	void Draw() override;
	void Animation() override;

	// 보스가 데미지를 입었는지 확인하는 함수
	void CheckDamaged();
	// 보스 패턴 선택을 위한 함수
	void ChoicePattern();

	bool BulletCountFire() override;

	void SetEnemyManager(EnemyManager * em) { _em = em; }
};

