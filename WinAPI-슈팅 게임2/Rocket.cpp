#include "stdafx.h"
#include "Rocket.h"


Rocket::Rocket()
{
}


Rocket::~Rocket()
{
}

HRESULT Rocket::Init()
{
	m_player = IMAGE->FindImage("rocket");
	m_speed = 5.0f;

	m_missile = new Missile;
	m_missile->Init(100, 500);

	m_delay = 0;

	return S_OK;
}

void Rocket::Release()
{
	SAFE_DELETE(m_missile);
}

void Rocket::Update()
{
	// 이동
	// 왼쪽
	if (INPUT->GetKey(VK_LEFT) &&
		m_player->GetX() > 0) {
		m_player->SetX(m_player->GetX() - m_speed);
	}
	// 오른쪽
	if (INPUT->GetKey(VK_RIGHT) &&
		(m_player->GetX() + m_player->GetWidth()) < WINSIZEX) {
		m_player->SetX(m_player->GetX() + m_speed);
	}
	// 위
	if (INPUT->GetKey(VK_UP) &&
		m_player->GetY() > 0) {
		m_player->SetY(m_player->GetY() - m_speed);
	}
	// 아래
	if (INPUT->GetKey(VK_DOWN) &&
		(m_player->GetY() + m_player->GetHeight()) < WINSIZEY) {
		m_player->SetY(m_player->GetY() + m_speed);
	}

	if (INPUT->GetKeyDown(VK_SPACE)) {
		m_missile->Fire(
			m_player->GetX() + m_player->GetWidth() / 2,
			m_player->GetY() - m_player->GetHeight() / 2);
	}

	m_missile->Update();
}

void Rocket::Render()
{
	m_missile->Render();
	m_player->Render(GetMemDC(), m_player->GetX(), m_player->GetY());
}
