#pragma once

#include "GameNode.h"
#include "TileMap.h"

// 타일 정보를 탱크한테 넘겨주면 탱크가 알아서 처리하게 할꺼

enum TANKDIRECTION {
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_DOWN
};

class TankMap : public GameNode
{
private:
	tagTile _tiles[TILEX * TILEY];
	// DWORD 특정값이 아닌 4byte로 된거 담는 녀석
	DWORD _attributes[TILEX * TILEY];
	int _pos[2];
public:
	TankMap();
	~TankMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void Load();

	tagTile* GetTiles() { return _tiles; }
	int GetPosFirst() { return 0; }
	int GetPosSecond() { return TILEX * TILEY - 1; }
	DWORD* GetAttribute() { return _attributes; }


	POINT GetOBJECTFrame(OBJECT obj);
};

