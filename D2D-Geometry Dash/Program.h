#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	class Camera* mainCamera;
	class BackGround* bg[2];
	class BackGround* bottom[2];
	class Rect* rect[2];

	LPDIRECT3DTEXTURE9 pTex[4];
	
	LPD3DXFONT font;

	bool isPause;
	int score = 0;

	Json::Value* root;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void Init();
	void Save();
	void Load();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);
};

/*
	과제
	마리오 뛰어다니고 점프하고 하는 거 구현하기
*/