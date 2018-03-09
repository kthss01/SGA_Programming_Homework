#pragma once
#include "GameNode.h"

class NoneScene : public GameNode
{
public:
	NoneScene();
	~NoneScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
};

