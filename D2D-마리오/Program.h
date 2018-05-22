#pragma once

// MainGame대신 돌아갈꺼

#define BOX_MAX 4
#define BOX2_MAX 29
#define BOX3_MAX 4

class Program
{
private:
	class Camera* mainCamera;
	class Rect* mario;
	class BackGround* bg;
	class Box* box[BOX_MAX];
	class Box* box2[BOX2_MAX];
	class Box* box3[BOX3_MAX];

	POINT mousePos;
	LPD3DXFONT font;

	LPDIRECT3DTEXTURE9 pTex[3];

	bool isDebug;
public:
	Program();
	~Program();

	void BoxInit();

	void Update();
	void Render();
};