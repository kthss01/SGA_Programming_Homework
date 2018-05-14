#pragma once
#include "GameObject.h"
class BackGround : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	float speed;

	LPD3DXEFFECT pEffect;
	LPDIRECT3DTEXTURE9 pTex;

	class Camera* camera;
public:
	BackGround();
	~BackGround();

	void Init();
	void Release();
	void Update();
	void Render();

	void SetSpeed(float speed) { this->speed = speed; }
	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 texture) { pTex = texture; }
	void SetColor(DWORD color) {
		vertice[0].color = color;
		vertice[1].color = color;
		vertice[2].color = color;
		vertice[3].color = color;
	}
	DWORD GetColor() { return vertice[0].color; }
};