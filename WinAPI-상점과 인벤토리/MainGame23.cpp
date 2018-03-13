#include "stdafx.h"
#include "MainGame23.h"

#include "ShopScene.h"

MainGame23::MainGame23()
{
}


MainGame23::~MainGame23()
{
}

HRESULT MainGame23::Init()
{
	GameNode::Init();
	isDebug = false;

	SCENE->AddScene("Shop", new ShopScene);

	SCENE->ChangeScene("Shop");

	return S_OK;
}

void MainGame23::Release()
{
	GameNode::Release();
	SCENE->Release();
}

void MainGame23::Update()
{
	GameNode::Update();

	SCENE->Update();

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame23::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		SCENE->Render();

	}
	//==================   Debug   ====================
	if (isDebug)
	{
		SetTextColor(GetMemDC(), RGB(255, 255, 255));
		sprintf_s(str, "x : %d y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 10, 10, str, strlen(str));
	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
