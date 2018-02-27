#pragma once

#include "GameNode.h"

#include <vector>
using namespace std;

#define POKEMONMAX 151

struct tagPokemon {
	float x, y;
	float angle;
	float speed;
	float radius;
};

class MainGame14 : public GameNode
{
private:
	tagPokemon m_pokemon[POKEMONMAX];
	vector<pair<int, int>> v;	// 포켓몬 머리들의 위치를 저장할 벡터(2차원 배열 위치로)

	Image * map;
	Image * pokemons;		// 12 * 13 크기의 포켓몬 머리 151마리 (마지막 줄 7마리)

public:
	MainGame14();
	~MainGame14();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void PokemonMove();
};

