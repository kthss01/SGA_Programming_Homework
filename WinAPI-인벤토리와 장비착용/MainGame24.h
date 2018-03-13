#pragma once

#include "GameNode.h"

class MainGame24 : public GameNode
{
private:
	bool isDebug;

	char str[128];
public:
	MainGame24();
	~MainGame24();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};

