#pragma once

// MainGame대신 돌아갈꺼

enum BodyPart {
	BODYPART_HEAD,
	BODYPART_BODY,
	BODYPART_LEFTARM,
	BODYPART_RIGHTARM,
	BODYPART_LEFTLEG,
	BODYPART_RIGHTLEG,
	BODYPART_END
};

class Program
{
private:
	class Camera* mainCamera;

	class Rect* bodyPart[BODYPART_END];
	class Transform* bottom;

	class BackGround* bg;
	LPDIRECT3DTEXTURE9 pTex2;

	int currentPart;

	LPDIRECT3DTEXTURE9 pTex[BODYPART_END];

	Json::Value* root;
	Json::Value* readJson;

	vector< vector<class Transform> > vTrans;
	bool aniStart;
	float deltaTime;
	int currentAni;
	int aniSize;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);

	void InitTransform(class Transform* pTrans);
	void SaveTransform(class Transform* pTrans, wstring name);
	void LoadTransform(class Transform* outTrans, wstring name);
	void SaveAnimation();
	void LoadAnimation();
};