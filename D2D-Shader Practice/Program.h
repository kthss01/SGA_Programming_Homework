#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	class Rect* rect;
	class BackGround* bg;
	class Camera* mainCamera;
	class BaseBackGround* baseBg;
	class MiniMap* miniMap;

	LPDIRECT3DTEXTURE9 pTex[3];

	// 텍스에 대한 정보를 저장할 곳
	LPDIRECT3DTEXTURE9 pRenderTexture;
	// 텍스에 대한 표면 (색상값 설정)
	LPDIRECT3DSURFACE9 pRenderSurface;

	int currentBg;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void CreateRenderTargetTexture();
};

/*
	과제
	마리오 뛰어다니고 점프하고 하는 거 구현하기
*/