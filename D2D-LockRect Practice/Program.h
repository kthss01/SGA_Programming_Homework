#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect[12*9];
	class Rect* tile[12*9];
	class Rect* resource;
	class Rect* mapUI;

	LPDIRECT3DTEXTURE9 pTex[4];

	Json::Value* root;
	Json::Value* readJson;

	LPD3DXSPRITE sprite;

	bool isTileLoad;
	int tileCnt;

	float deltaTime;
public:
	Program();
	~Program();

	void LoadTexture();

	void Update();
	void Render();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);
};
