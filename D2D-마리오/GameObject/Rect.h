#pragma once
#include "GameObject.h"

#define MAX_SHADOW 10

#define SPEED 2.5f
#define GRAVITY 0.5f
#define JUMP -10

enum STATUS {
	STATUS_NORMAL,
	STATUS_ONLAND,
	STATUS_TOUP,
	STATUS_TOLOW,
	STATUS_END
};

class Rect : public GameObject
{
private:
	STATUS status;
	float vy;
	int isLeft;
	float currentIndexX, currentIndexY;
	Vector2 bottom;

	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex;
	LPD3DXEFFECT pEffect;
	float deltaTime;

	class Camera* camera;
	class AnimationClip* clips[STATUS_END*2];

	Transform* shadows[MAX_SHADOW];

	int shadowCnt;
	bool check;
public:
	Rect();
	~Rect();

	void Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot = Vector2(0, 0));
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void InitAnimation();

	void Jump();
	void Move();

	void DrawInterface();
	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }
};