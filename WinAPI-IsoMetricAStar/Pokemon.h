#pragma once

#include "GameNode.h"
#include "PokemonMap.h"

struct tagAStarTile {
	bool walkable;
	bool listOn;
	int isoX, isoY;
	int F, G, H;
	tagAStarTile * parent;
};

enum POKEMONDIRECTION {
	POKEMONDIRECTION_DOWN,
	POKEMONDIRECTION_UP,
	POKEMONDIRECTION_LEFT,
	POKEMONDIRECTION_RIGHT,
	POKEMONDIRECTION_LEFTDOWN,
	POKEMONDIRECTION_RIGHTDOWN,
	POKEMONDIRECTION_LEFTUP,
	POKEMONDIRECTION_RIGHTUP,
};

enum POKEMONSTATE {
	POKEMONSTATE_IDLE,
	POKEMONSTATE_MOVE
};

class Pokemon : public GameNode
{
private:
	POKEMONDIRECTION _direction;
	POKEMONSTATE _state;
	Image * _image;
	RECT _rc;
	float _x, _y;
	int frameX, frameY;
	int _startX, _startY;
	float _speed;
	int _cnt;

	tagAStarTile * _targetTile;

	tagAStarTile _aStarTile[TILE_COUNT_X][TILE_COUNT_Y];

	vector<tagAStarTile*> _openList;
	vector<tagAStarTile*> _closeList;

	int _lastIndex;
	int _Cx, _Cy, _Cg;

	int _aStarStartX, _aStarStartY;
	int _endX, _endY;

	PokemonMap * _map;
public:
	Pokemon();
	~Pokemon();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	int FindDirectionFrame(POKEMONDIRECTION dir);
	void PokemonMove();
	void SetPokemonMapMemoryLink(PokemonMap * map) { _map = map; }

	int GetCornerIndex(int isoX, int isoY);
	bool IsInRhombus(int corner, int isoX, int isoY);

	void PathInit();
	void PathFind();
};

