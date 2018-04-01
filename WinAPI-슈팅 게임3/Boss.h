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

class Boss : public Enemy
{
private:
	Image * _images[BOSS_END];

	int _frameCount;
	int _currentFrameXs[BOSS_END];

	bool _isHatch;
	bool _isHatchClose;
	bool _isDeploy;
	bool _isDeployClose;
	bool _isFire;
	bool _isEnemy;

	BOSS_PARTS currentParts;

	int _hp;
	int _maxHp;
	ProgressBar * _hpBar;
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

	void CheckDamaged();
};

