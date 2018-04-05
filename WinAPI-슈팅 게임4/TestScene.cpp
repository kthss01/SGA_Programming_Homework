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

	test.push_back("test");
	test.push_back("test1");
	test.push_back("test2");
	test.push_back("test3");


	TEXTDATA->TextSave((char*)"save/test.txt", test);

	test = TEXTDATA->TextLoad((char*)"save/test.txt");

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
		for (int i = 0; i < test.size(); i++) {
			TextOut(GetMemDC(), WINSIZEX / 2, i * 20,
				test[i].c_str(), strlen(test[i].c_str()));
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
}
