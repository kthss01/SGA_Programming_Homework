#pragma once

#include "GameNode.h"
#include "IsoTile.h"

class SubIsoMap : public GameNode
{
private:
	bool isDebug;

	RECT rc[TILE_SIZE_X][TILE_SIZE_Y];
public:
	SubIsoMap();
	~SubIsoMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
};

