#pragma once

// MainGame대신 돌아갈꺼

#define BOX_MAX 4
#define BOX2_MAX 29
#define BOX3_MAX 4
#define COIN_MAX 5

class Program
{
private:
	class Camera* mainCamera;
	class Rect* mario;
	class BackGround* bg;
	class Box* box[BOX_MAX];
	class Box* box2[BOX2_MAX];
	class Box* box3[BOX3_MAX];
	class Coin* coin[COIN_MAX];

	POINT mousePos;
	LPD3DXFONT font;

	LPDIRECT3DTEXTURE9 pTex[3];

	bool isDebug;
public:
	Program();
	~Program();

	void BoxInit();
	void CoinInit();

	void Update();
	void Render();
};