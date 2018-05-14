#pragma once
#include "GameObject.h"
class BackGround : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		Vector2 uv;
	};
	Vertex vertice[4];

	float speed;

	LPDIRECT3DTEXTURE9 pTex;
public:
	BackGround();
	~BackGround();

	void Init();
	void Release();
	void Update();
	void Render();

	void SetSpeed(float speed) { this->speed = speed; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { pTex = texture; }
};