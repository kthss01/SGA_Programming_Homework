#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect;
	class BackGround* bg;

	LPDIRECT3DTEXTURE9 pTex[2];
public:
	Program();
	~Program();

	void Update();
	void Render();
};

/*
	과제
	마리오 뛰어다니고 점프하고 하는 거 구현하기
*/