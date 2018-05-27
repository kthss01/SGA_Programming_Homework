#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

	mainCamera = new Camera;

	//HRESULT hr = D3DXCreateTextureFromFile(
	//	D2D::GetDevice(),
	//	L"Textures/resource.png",
	//	//L"Textures/Box.png",
	//	&pTex
	//);

	LoadTexture();

	D3DLOCKED_RECT lockRect;
	pTex[0]->LockRect(0, &lockRect, NULL, D3DLOCK_READONLY);
	DWORD* color = (DWORD*)lockRect.pBits;

	for (int i = 0; i < 12 * 9; i++) {
		rect[i] = new Rect;
		rect[i]->SetCamera(mainCamera);
		D3DXCOLOR destColor = color[i%12 + i / 12 * 32];
		rect[i]->Init(L"./Shader/BaseColor.fx",
			Vector2(-25, -25), Vector2(25, 25), destColor, false);
		rect[i]->GetTransform()->SetScale(Vector2(0.01f, 0.01f));
		rect[i]->GetTransform()->SetWorldPosition(
			Vector2(WINSIZE_X / 2 - 100, -WINSIZE_Y / 2 + 120));
		rect[i]->SetInterPolation(true);

		tile[i] = new Rect;
		tile[i]->SetCamera(mainCamera);
		tile[i]->Init(L"./Shader/ColorTexture.fx",
			Vector2(-25, -25), Vector2(25, 25), 0xffffffff, true);
		tile[i]->SetTexture(pTex[3]);
		tile[i]->GetTransform()->SetWorldPosition(
			Vector2(
				-400 + i % 12 * 50,
				-100 + i / 12 * 50));

		rect[i]->SetTileTrans(tile[i]->GetTransform());
	}

	pTex[0]->UnlockRect(0);

	mapUI = new Rect;
	mapUI->SetCamera(mainCamera);
	mapUI->Init(L"./Shader/ColorTexture.fx",
		Vector2(-100, -100), Vector2(100, 100), 0xffffffff, true);
	mapUI->SetTexture(pTex[2]);
	mapUI->GetTransform()->SetWorldPosition(
		Vector2(WINSIZE_X / 2 - 100, -WINSIZE_Y / 2 + 100));

	resource = new Rect;
	resource->SetCamera(mainCamera);
	resource->Init(L"./Shader/ColorTexture.fx", 
		Vector2(-60, -45), Vector2(60, 45), 0xffffffff, true);
	resource->SetTexture(pTex[0]);
	resource->GetTransform()->SetWorldPosition(
		Vector2(WINSIZE_X / 2 - 100, -WINSIZE_Y / 2 + 120));

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	//SOUND->Play("Test");

	D3DXCreateSprite(D2D::GetDevice(), &sprite);

	root = new Json::Value();
	readJson = new Json::Value();
	//float time = FRAME->GetNowRealTimeSec();
	//Json::SetValue(*root, "test", time);
	//WriteJsonData(L"Test.Json", root);
	//ReadJsonData(L"Test.Json", readJson);
	//float temp;
	//Json::GetValue(*readJson, "test", temp);

	isTileLoad = false;
	tileCnt = 0;
	deltaTime = 0;
}

Program::~Program()
{
	for (int i = 0; i < 12 * 9; i++) {
		rect[i]->Release();
		SAFE_DELETE(rect[i]);

		tile[i]->Release();
		SAFE_DELETE(tile[i]);
	}

	for (int i = 0; i < 4; i++)
		SAFE_RELEASE(pTex[i]);
	SAFE_RELEASE(sprite);

	mapUI->Release();
	SAFE_DELETE(mapUI);

	resource->Release();
	SAFE_DELETE(resource);

	SAFE_DELETE(mainCamera);

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);
}

void Program::LoadTexture()
{
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileEx(
		D2D::GetDevice(),
		L"./Textures/resource.png", 
		D3DX_FROM_FILE,
		D3DX_FROM_FILE,
		NULL,
		D3DUSAGE_DYNAMIC,	// 읽어서 사용하려면 dynamic으로 써야함
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		//D3DX_DEFAULT,	// 이 두개 때문에 픽셀 사이에 처리가 이상해진거
		D3DX_FILTER_LINEAR,
		//D3DX_DEFAULT,	// 
		D3DX_FILTER_LINEAR,
		NULL,
		&info,
		NULL,
		&pTex[0]
	);
	assert(SUCCEEDED(hr));

	//HRESULT hr = D3DXCreateTextureFromFile(
	//	D2D::GetDevice(),
	//	L"./Textures/resource.png",
	//	&pTex[0]
	//);
	//assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFileEx(
		D2D::GetDevice(),
		L"Textures/mapIcon.png",
		60, 35,
		D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, 0xffff0000, NULL, NULL,
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/map.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));


	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/tile.png",
		&pTex[3]
	);
	assert(SUCCEEDED(hr));
}

void Program::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE)) {
		isTileLoad = true;
	}

	if (isTileLoad) {
		deltaTime += FRAME->GetFrameDeltaSec();
		if (deltaTime > 0.2f) {
			deltaTime = 0;
			
			Vector2 tempScale = tile[tileCnt]->GetTransform()->GetScale();
			Vector2 tempPos = tile[tileCnt]->GetTransform()->GetWorldPosition();

			rect[tileCnt]->GetTransform()->SetScale(tempScale);
			rect[tileCnt]->GetTransform()->SetWorldPosition(tempPos);
			
			tileCnt++;
			if (tileCnt >= 12 * 9)
				isTileLoad = false;
		}
	}

	mainCamera->UpdateCamToDevice();
	for (int i = 0; i < 12 * 9; i++) {
		rect[i]->Update();
		tile[i]->Update();
	}
	mapUI->Update();
	resource->Update();
}

void Program::Render()
{
	for (int i = 0; i < 12 * 9; i++) {
		tile[i]->Render();
		rect[i]->Render();
	}

	mapUI->Render();

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	{
		RECT rc = { 0, 0, 60, 35 };
		sprite->Draw(pTex[1], &rc, &D3DXVECTOR3(0,0,0), 
			&D3DXVECTOR3(WINSIZE_X - 80, 15, 0), 0xffffffff);
	}
	sprite->End();

	resource->Render();

	//Vector2 center = Vector2(0, 0);
	//GIZMO->Circle(
	//	center, 100.0f, 0xFF808000
	//);
}

void Program::WriteJsonData(wstring fileName, Json::Value * root)
{
	ofstream stream;
	string temp = String::WStringToString(fileName);
	stream.open(temp);
	{
		Json::StyledWriter writer;
		stream << writer.write(*root);
	}
	stream.close();
}

void Program::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}