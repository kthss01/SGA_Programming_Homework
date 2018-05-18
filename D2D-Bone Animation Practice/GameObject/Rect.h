#pragma once
#include "GameObject.h"

#define BODYPART_DISTANCE 1000

enum BodyKind {
	BodyKind_Body,
	BodyKind_Head,
	BodyKind_LeftArm,
	BodyKind_LeftLeg,
	BodyKind_RightArm,
	BodyKind_RightLeg,
	BodyKind_End
};

class Rect : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		//DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex[2];
	LPD3DXEFFECT pEffect;
	float deltaTime;

	class Camera* camera;
	class AnimationClip* clips;

	Transform* child[BodyKind_End];

	bool isUnion;
	bool isDivine;
	int distance;

	float angle;
public:
	Rect();
	~Rect();

	void Init();
	void Release();
	void Update();
	void Render();
	void RenderRect();
	
	void SettingBodyPart();

	void DrawInterface();
	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);

	void MoveToBodyPart();

	void SetCamera(Camera* camera) { this->camera = camera; }
};