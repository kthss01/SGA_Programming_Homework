#pragma once

#include "GameNode.h"
#include "Bullet.h"

// 그림자 위치 조정 쉽게 하기 위해서
#define SHADOWX -25
#define SHADOWY -35

// 에너미 매니저 전방선언
class EnemyManager;
class ResultScene;

class Rocket : public GameNode
{
private:
	Image * m_player;
	Image * m_shadow;

	Missile* m_missile;

	float m_speed;

	float m_maxHp;
	float m_hp;
	RECT m_hpBar;

	HBRUSH brush;
	HFONT hFont;

	EnemyManager * m_em;
	ResultScene * m_resultScene;
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

	void SetEnemyManager(EnemyManager * em) { m_em = em; }
	void SetResultScene(ResultScene * resultScene) { m_resultScene = resultScene; }
};