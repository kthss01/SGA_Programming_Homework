#pragma once
#include "GameObject.h"

#define SPEED 2.5f
#define GRAVITY 0.5f
#define JUMP -10

enum STATUS {
	STATUS_NORMAL,
	STATUS_ONLAND,
	STATUS_TOUP,
	STATUS_TOLOW,
};

class Rect : public GameObject
{
private:
	STATUS status;
	float vy;
	bool isLeft;
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

	Transform * bgTransform;
	class Camera* camera;

	int currentBg;
	bool isSee;
public:
	Rect();
	~Rect();

	void Init();
	void Release();
	void Update();
	void Render();

	void Jump();
	void Move();

	void DrawInterface();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetBgTransform(Transform* transform) { bgTransform = transform; }

	int GetCurrentBg() { return currentBg; }
};