#pragma once

#include "GameNode.h"

class MainGame23 : public GameNode
{
private:
	bool isDebug;

	char str[128];
public:
	MainGame23();
	~MainGame23();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};

