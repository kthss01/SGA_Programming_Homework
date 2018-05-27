#pragma once
#include "GameObject.h"

#define TEMPSIZE 1

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

	class Camera* camera;
	class AnimationClip* clips;

	Transform* tempTrans[TEMPSIZE];

	RectCollider* tempCollider;

	bool isTexture;
	bool isInterpolation;

	float delta;

	Transform* tileTrans;
public:
	Rect();
	~Rect();

	void Init(wstring shaderFile, const Vector2 min, const Vector2 max, 
		const DWORD color, bool isTex);
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void InitShader(wstring shaderFile);
	void InitVertex(DWORD color, Vector2 min, Vector2 max);
	void InitBuffer();

	void DrawInterface();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }
	void SetTransform(Transform* transform) { this->transform = transform; }
	
	void SetInterPolation(bool isInterPolation) { this->isInterpolation = isInterPolation; }

	void SetTileTrans(Transform* tileTrans) { this->tileTrans = tileTrans; }
};