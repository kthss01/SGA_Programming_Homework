#include "stdafx.h"
#include "PokemonWorld.h"


PokemonWorld::PokemonWorld()
{
}


PokemonWorld::~PokemonWorld()
{
}

HRESULT PokemonWorld::Init()
{
	map = new PokemonMap;
	map->Init();

	pokemon = new Pokemon;
	pokemon->SetPokemonMapMemoryLink(map);
	pokemon->Init();

	return S_OK;
}

void PokemonWorld::Release()
{
	SAFE_DELETE(pokemon);
	SAFE_DELETE(map);
}

void PokemonWorld::Update()
{
	map->Update();
	pokemon->Update();
}

void PokemonWorld::Render()
{
	map->Render();
	pokemon->Render();
}
