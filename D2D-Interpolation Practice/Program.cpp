#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("BGM", "sounds/bgm.mp3", true, true);

	mainCamera = new Camera;

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/goku_9x1.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	bg = new BackGround;
	bg->SetTexture(pTex[0]);
	bg->Init();
	bg->SetCamera(mainCamera);

	rect = new Rect;
	rect->SetTexture(pTex[1]);
	rect->Init();
	rect->SetCamera(mainCamera);

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	//SOUND->Play("Test");
}

Program::~Program()
{
	bg->Release();
	rect->Release();

	SAFE_RELEASE(pTex[0]);
	SAFE_RELEASE(pTex[1]);
	
	SAFE_DELETE(rect);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE)) {
		if (!SOUND->IsPlaySound("BGM"))
			SOUND->Play("BGM");
	}

	mainCamera->UpdateCamToDevice();
	bg->Update();
	rect->Update();
}

void Program::Render()
{
	bg->Render();
	rect->Render();
}
