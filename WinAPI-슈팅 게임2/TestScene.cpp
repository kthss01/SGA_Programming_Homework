#include "stdafx.h"
#include "TestScene.h"


TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Init()
{
	isDebug = false;

	return S_OK;
}

void TestScene::Release()
{
}

void TestScene::Update()
{
	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void TestScene::Render()
{
	//=================================================
	{

	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
}
