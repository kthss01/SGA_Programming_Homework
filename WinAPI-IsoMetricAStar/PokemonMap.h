#pragma once

#include "GameNode.h"
#include "IsoTile.h"

class PokemonMap : public GameNode
{
private:
	int _startX;
	int _startY;

	char str[128];
	bool _isDebug;

	tagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];
public:
	PokemonMap();
	~PokemonMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void DrawRhombus(int left, int top);
	void DrawTile();
	void Load();

	tagTile (* GetTiles())[TILE_COUNT_Y] { return _tileMap; }
	POINT GetIsoPos(int isoX, int isoY) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH) + RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT) + RADIUS_HEIGHT;
		return PointMake(left, top);
	}
	POINT GetStartPos() { return PointMake(_startX, _startY); }
};

