#pragma once
#include "GameObject.h"

#define JUMP -10.0f
#define GRAVITY 2.0f

enum State {
	STATE_WALK,
	STATE_TURN,
	STATE_RUN,
	STATE_RUNTURN,
	STATE_JUMP,
	STATE_RUNJUMP,
	STATE_JUMPUP,
	STATE_JUMPDOWN,
	STATE_CLIMB,
	STATE_DRINK,
	STATE_END,
	STATE_IDLE
};

class Rect : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex[STATE_END];
	float num[STATE_END];
	float currentIndex;
	float deltaTime;

	bool isLeft;
	State currentState;
	float speed;

	float vy;
	Vector2 land;
public:
	Rect();
	~Rect();

	void Init();
	void Release();
	void Update();
	void Render();

	void LoadTexture();
	void DrawInterface();

	void Move();
	void Jump();
};