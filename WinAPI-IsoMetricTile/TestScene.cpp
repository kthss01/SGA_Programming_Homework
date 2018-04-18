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

	//SOUND->Play("Test", 0.5f);
	//
	//test.push_back("test");
	//test.push_back("test1");
	//test.push_back("test2");
	//test.push_back("test3");
	//
	//TEXTDATA->TextSave((char*)"save/test.txt", test);
	//
	//test = TEXTDATA->TextLoad((char*)"save/test.txt");
	//
	//INIDATA->AddData("Test", "KeyTest", "1000");
	//INIDATA->AddData("Test", "KeyTest2", "3.141592f");
	//INIDATA->AddData("Test2", "KeyTest3", "1234");
	//INIDATA->AddData("Test3", "KeyTest4", "1000");
	//INIDATA->AddData("Test", "KeyTest5", "1000");
	//INIDATA->SaveData("IniSave");

	return S_OK;
}

void TestScene::Release()
{
}

void TestScene::Update()
{
	SOUND->Update();

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
		//for (int i = 0; i < test.size(); i++) {
		//	TextOut(GetMemDC(), WINSIZEX / 2, i * 20,
		//		test[i].c_str(), strlen(test[i].c_str()));
		//}
		//
		//sprintf_s(str, "%d",
		//	INIDATA->LoadDataInteger("IniSave", "Test3", "KeyTest4"));
		//TextOut(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));
		//
		//float data = atof(INIDATA->LoadDataString("IniSave", "Test", "KeyTest2"));
		//sprintf_s(str, "%f", data);
		//TextOut(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 50, str, strlen(str));

	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
}
