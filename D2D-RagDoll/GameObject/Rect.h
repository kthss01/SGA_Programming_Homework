#pragma once
#include "GameObject.h"
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

	class Camera* camera;

	bool colliderCheck;

	Transform* tempTrans;
public:
	Rect();
	~Rect();

	void Init(wstring shaderFile, const Vector2 uv, 
		const Vector2 min, const Vector2 max, const Vector2 pivot = Vector2(0, 0));
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void DrawInterface();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }

	bool isMouseCollision();
};