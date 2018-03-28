#pragma once

#include "GameNode.h"
#include "Bullet.h"

#define SHADOWX 25
#define SHADOWY 25

class Rocket : public GameNode
{
private:
	Image * m_player;
	Image * m_shadow;

	Missile* m_missile;

	float m_speed;
	int m_delay;

	float m_maxHp;
	float m_hp;
	RECT m_hpBar;

	HBRUSH brush;
	HFONT hFont;
public:
	Rocket();
	~Rocket();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	Image* GetImage() { return m_player; }
	Missile* GetMissile() { return m_missile; }

	RECT GetRect() {
		return RectMake(
			m_player->GetX(), m_player->GetY(),
			m_player->GetWidth(), m_player->GetHeight());
	}

	float GetHp() { return m_hp; }
	void SetHp(float hp) { m_hp = hp; }
};