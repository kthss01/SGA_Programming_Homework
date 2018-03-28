#pragma once

#include "GameNode.h"
#include "Bullet.h"

class Rocket : public GameNode
{
private:
	Image * m_player;

	Missile* m_missile;

	float m_speed;
	int m_delay;
public:
	Rocket();
	~Rocket();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	Image* GetImage() { return m_player; }
};