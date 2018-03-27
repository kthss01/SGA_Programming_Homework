#pragma once

#include "GameNode.h"
#include "Bullet.h"
#include "Enemy.h"

class Rocket : public GameNode
{
private:
	Image * m_player;

	Missile* m_missile;

	Enemy* m_enemy;

	float m_speed;
	int m_delay;
public:
	Rocket();
	~Rocket();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};

