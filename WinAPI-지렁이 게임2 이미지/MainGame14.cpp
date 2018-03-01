#include "stdafx.h"
#include "MainGame14.h"


MainGame14::MainGame14()
{
}


MainGame14::~MainGame14()
{
}

HRESULT MainGame14::Init()
{
	GameNode::Init();

	// 초기화 해주는 함수
	memset(m_pokemon, 0, sizeof(m_pokemon));
	//memset(m_pokemon, 0, sizeof(tagBead) * BEADMAX);

	for (int i = 0; i < POKEMONMAX; i++) {
		m_pokemon[i].radius = 30.0f;
		m_pokemon[i].speed = 5.0f;
		// 삼항 연산자 (조건) ? 참 : 거짓;
		m_pokemon[i].angle = (i == 0) ? RND->GetInt(7) : m_pokemon[i - 1].angle;
		m_pokemon[i].x = (i == 0) ? RND->GetFromInto(100, WINSIZEX - 100)
			: m_pokemon[i].x - cosf(m_pokemon[i].angle) *
			(m_pokemon[i - 1].radius * 2 + m_pokemon[i].radius * 2);
		m_pokemon[i].y = (i == 0) ? RND->GetFromInto(100, WINSIZEY - 100)
			: m_pokemon[i].y - (-sinf(m_pokemon[i].angle)) *
			(m_pokemon[i - 1].radius * 2 + m_pokemon[i].radius * 2);
	}

	map = new Image;
	pokemons = new Image;

	map->Init("images/map.bmp", WINSIZEX, WINSIZEY);
	pokemons->Init(
		"images/pokemon.bmp", WINSIZEX, WINSIZEY, true, RGB(255,0,255));

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 13 && j == 7) break;	// 151마리까지만 그 이후는 break
			v.push_back(make_pair(i, j));	// 151마리의 위치를 다 넣어둠
		}
	}

	

	return S_OK;
}

void MainGame14::Release()
{
	GameNode::Release();
}

void MainGame14::Update()
{
	GameNode::Update();

	if (INPUT->GetKey(VK_LEFT)) {
		m_pokemon[0].angle += 0.05f;
	}
	if (INPUT->GetKey(VK_RIGHT)) {
		m_pokemon[0].angle -= 0.05f;
	}

	PokemonMove();
}

void MainGame14::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	map->Render(memDC);
	// test
	//pokemons->Render(memDC);

	// 포켓몬 머리 하나 85, 59

	//// 테스트2
	//for (int i = 0; i < 12; i++) {
	//	for(int j=0; j<12;j++)
	//		pokemons->Render(memDC, 85*i, 85*j, 85*i, 59*j, 85, 59);
	//}

	for (int i = 0; i < POKEMONMAX; i++) {
		//EllipseMakeCenter(memDC, m_pokemon[i].x, m_pokemon[i].y, m_pokemon[i].radius);

		pokemons->Render(memDC, m_pokemon[i].x, m_pokemon[i].y, 
			85*v[i].second, 59*v[i].first, 85, 59);
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame14::PokemonMove()
{
	for (int i = 0; i < POKEMONMAX; i++) {
		if (i == 0) {
			m_pokemon[i].x += cosf(m_pokemon[i].angle) * m_pokemon[i].speed;
			m_pokemon[i].y += -sinf(m_pokemon[i].angle) * m_pokemon[i].speed;

			// 벽 충돌
			// 위치 보정 필요성은
			// 속도가 너무 높았을 때 뛰쳐나가지 않게 하는거임
			// 속도 조절 하면 안해도 되는 경우
			// 왼쪽 
			if (m_pokemon[i].x - m_pokemon[i].radius < 0) {
				m_pokemon[i].x = m_pokemon[i].radius;
				m_pokemon[i].angle = PI - m_pokemon[i].angle;
			}
			// 오른쪽
			if (m_pokemon[i].x + m_pokemon[i].radius > WINSIZEX) {
				m_pokemon[i].x = WINSIZEX - m_pokemon[i].radius;
				m_pokemon[i].angle = PI - m_pokemon[i].angle;
			}
			// 위
			if (m_pokemon[i].y - m_pokemon[i].radius < 0) {
				m_pokemon[i].y = m_pokemon[i].radius;
				m_pokemon[i].angle = -m_pokemon[i].angle;
			}
			// 아래
			if (m_pokemon[i].y + m_pokemon[i].radius > WINSIZEY) {
				m_pokemon[i].y = WINSIZEY - m_pokemon[i].radius;
				m_pokemon[i].angle = -m_pokemon[i].angle;
			}
		}
		else {
			// 이런식으로 해두 되는데 컴퓨터 성능에 다라 
			// 한꺼번에 회전하는 경우도 있음 이 방식은 좋지 않음
			//m_pokemon[i-1].angle = m_pokemon[i-1].angle;

			float distance = GetDistance(m_pokemon[i].x, m_pokemon[i].y,
				m_pokemon[i - 1].x, m_pokemon[i - 1].y);

			// m_pokemon[i].radius + m_pokemon[i-1].radius
			if (m_pokemon[i].radius * 2 < distance) {

				// GetAngle 넣을때 인수 반대로 넣으면 각도 차 날 수 있음 180 정도
				// 그래서 포탄 발사 때 원끼리 충돌할 때 반대로 넣는거
				m_pokemon[i].angle = GetAngle(
					m_pokemon[i].x, m_pokemon[i].y, m_pokemon[i - 1].x, m_pokemon[i - 1].y);

				m_pokemon[i].x = m_pokemon[i - 1].x - cosf(m_pokemon[i].angle) *
					m_pokemon[i].radius * 2;
				m_pokemon[i].y = m_pokemon[i - 1].y - (-sinf(m_pokemon[i].angle)) *
					m_pokemon[i].radius * 2;
			}

			m_pokemon[i].x += cosf(m_pokemon[i].angle) * m_pokemon[i].speed;
			m_pokemon[i].y += -sinf(m_pokemon[i].angle) * m_pokemon[i].speed;
		}
	}
}
