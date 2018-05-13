#pragma once

#include "GameObject/GameObject.h"

class BaseBackGround : GameObject
{
private:
	struct Vertex {
		Vector2 position;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex;
public:
	BaseBackGround();
	~BaseBackGround();

	void Init();
	void Release();
	void Update();
	void Render();

	void SetTexture(LPDIRECT3DTEXTURE9 texture) { pTex = texture; }
};

