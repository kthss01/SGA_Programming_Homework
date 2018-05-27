#pragma once

// MainGame대신 돌아갈꺼

#define GRAVITY 5.0f

class Program
{
private:
	class Camera* mainCamera;
	class Rect* bottom;

	LPDIRECT3DTEXTURE9 pTex[5];

	vector<class Rect*> vRect;

	Json::Value* root;
	Json::Value* readJson;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void CreateRect();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);
};
