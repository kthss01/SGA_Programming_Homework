#pragma once

#include "GameNode.h"
#include "PokemonMap.h"
#include "Pokemon.h"

class PokemonWorld : public GameNode
{
private:
	Pokemon * pokemon;
	PokemonMap* map;
public:
	PokemonWorld();
	~PokemonWorld();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
};

