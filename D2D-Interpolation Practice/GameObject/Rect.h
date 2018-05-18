#pragma once
#include "GameObject.h"

enum STATUS {
	STATUS_IDLE,
	STATUS_DASH,
	STATUS_BACK,
	STATUS_END
};

#define SPEED 3.0f
#define MAXSHADOW 15

class Rect : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex;
	LPD3DXEFFECT pEffect;
	float deltaTime;

	STATUS status;
	int currentShadow;

	class Camera* camera;
	class AnimationClip* clips[STATUS_END];

	Transform* tempTrans[MAXSHADOW];
public:
	Rect();
	~Rect();

	void Init();
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void DrawInterface();
	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }
};