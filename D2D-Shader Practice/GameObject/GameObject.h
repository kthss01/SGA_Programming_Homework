#pragma once

#include "../Common/Transform.h"

class GameObject
{
protected:
	class Transform* transform;

	int stride;	// 한 사이즈
	DWORD FVF;

	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DINDEXBUFFER9 ib;
public:
	GameObject();
	~GameObject();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	Transform * GetTransform() { return transform; }
};

