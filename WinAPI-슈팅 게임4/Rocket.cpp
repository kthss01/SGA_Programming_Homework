#include "stdafx.h"
#include "Rocket.h"

#include "EnemyManager.h"
#include "ResultScene.h"

Rocket::Rocket()
{
}


Rocket::~Rocket()
{
}

HRESULT Rocket::Init()
{
	m_player = IMAGE->FindImage("rocket");
	m_shadow = IMAGE->FindImage("shadow");
	m_speed = 5.0f;

	m_missile = new Missile;
	m_missile->Init(100, 500);

	m_maxHp = 100.0f;
	m_hp = SAVEBOOK->GetHp();

	m_player->SetX(SAVEBOOK->GetPos().x);
	m_player->SetY(SAVEBOOK->GetPos().y);

	m_hpBar = RectMake(
		m_player->GetX(), m_player->GetY() - 5,
		m_player->GetWidth() * m_hp / m_maxHp, 5);
	
	m_shadow->SetX(m_player->GetX() + m_player->GetWidth() + SHADOWX);
	m_shadow->SetY(m_player->GetY() + m_player->GetHeight() + SHADOWY);

	SOUND->Play("bgm", 0.8f);

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

	m_hpBar = RectMake(
		m_player->GetX(), m_player->GetY() - 5,
		m_player->GetWidth() * m_hp / m_maxHp, 5);

	// 그림자 이미지의 위치는 플레이어 이미지 위치 오른쪽 아래 위치하도록
	m_shadow->SetX(m_player->GetX() + m_player->GetWidth() + SHADOWX);
	m_shadow->SetY(m_player->GetY() + m_player->GetHeight() + SHADOWY);

	// 스페이스 버튼 클릭시
	if (INPUT->GetKeyDown(VK_SPACE)) {
		// 미사일 총알을 플레이어 위치 위에서 발사
		m_missile->Fire(
			m_player->GetX() + m_player->GetWidth() / 2,
			m_player->GetY() - m_player->GetHeight() / 2);
		SOUND->Play("rocket_fire", 0.1f);
	}

	m_missile->Update();

	if (m_missile->CheckCollision(SAVEBOOK->GetRect())) {
		if (!SAVEBOOK->GetIsSave()) {
			SAVEBOOK->SetHp(m_hp);
			SAVEBOOK->SetPos(PointMake(m_player->GetX(), m_player->GetY()));
			SAVEBOOK->SetIsSave(true);
		}
	}

	// hp 0 시 결과 씬으로 이동
	if (m_hp == 0) {
		m_em->Release();
		m_resultScene->SetImage(IMAGE->FindImage("gameover"));
		SCENE->ChangeScene("Result");

		if (SOUND->IsPlaySound("bgm"))
			SOUND->Stop("bgm");
		if (SOUND->IsPlaySound("boss_bgm"))
			SOUND->Stop("boss_bgm");
		SOUND->Play("gameover", 0.8f);
		SAVEBOOK->AddGameOver();
	}
}

void Rocket::Render()
{
	m_missile->Render();

	m_shadow->Render(GetMemDC(), m_shadow->GetX(), m_shadow->GetY());
	m_player->Render(GetMemDC(), m_player->GetX(), m_player->GetY());

	BeginSolidColor(GetMemDC(), &brush, RGB(0, 128, 0));
	RectangleMake(GetMemDC(), m_hpBar);
	DeleteObject(brush);

	BeginSolidColor(GetMemDC(), &brush, RGB(235, 235, 235));
	DeleteObject(brush);

	//if (m_hp == 0) {
	//	SetTextAlign(GetMemDC(), TA_CENTER);
	//	SetBkMode(GetMemDC(), TRANSPARENT);

	//	BeginCreateFont(GetMemDC(), &hFont, 50);
	//	TextOut(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2, 
	//		"Game Over", strlen("Game Over"));
	//	DeleteObject(hFont);

	//	KillTimer(g_hWnd, 1);
	//}
}
