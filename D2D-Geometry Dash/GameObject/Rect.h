#pragma once
#include "GameObject.h"

#define GRAVITY 0.5f
#define JUMP -12.5f

enum STATUS {
	STATUS_IDLE,
	STATUS_JUMP
};

class Rect : public GameObject
{
private:
	STATUS status;
	float vy;
	Vector2 bottom;
	bool isPlayer;

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
	class AnimationClip* clips;
public:
	Rect();
	~Rect();

	void Init();
	void Release();
	void Update();
	void Render();

	void DrawInterface();

	void Jump();

	void SetPlayer(bool isPlayer) { this->isPlayer = isPlayer; }
	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { pTex = texture; }

	int GetStatus() { return status; }
	float GetVy() { return vy; }
	float GetBottomY() { return bottom.y; }

	void SetStatus(STATUS status) { this->status = status; }
	void SetVy(float vy) { this->vy = vy; }
	void SetBottomY(float y) { bottom.y = y; }
};