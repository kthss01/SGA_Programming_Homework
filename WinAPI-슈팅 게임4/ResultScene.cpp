#include "stdafx.h"
#include "ResultScene.h"

#include "EnemyManager.h"
#include "MainGame.h"

ResultScene::ResultScene()
{
}


ResultScene::~ResultScene()
{
}

HRESULT ResultScene::Init()
{
	isShow = true;
	//em->Release();

	return S_OK;
}

void ResultScene::Release()
{
}

void ResultScene::Update()
{
	if (INPUT->GetKeyDown(VK_RETURN)) {
		mg->ReStart();
		isShow = false;
	}
}

void ResultScene::Render()
{
	if(isShow)
		this->bg->Render(GetMemDC());
}
