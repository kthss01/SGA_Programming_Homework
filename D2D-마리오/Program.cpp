#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

	mainCamera = new Camera;

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/mario_all.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.jpg",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	mario = new Rect;
	mario->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	mario->SetTexture(pTex[0]);
	mario->SetCamera(mainCamera);
	
	bg = new BackGround;
	bg->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	bg->SetTexture(pTex[1]);
	bg->SetCamera(mainCamera);

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
	mario->Release();
	bg->Release();
	
	for (int i = 0; i < 3; i++)
		SAFE_RELEASE(pTex[i]);
	
	SAFE_DELETE(mario);
	SAFE_DELETE(bg);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();
	bg->Update();
	mario->Update();
}

void Program::Render()
{
	bg->Render();
	mario->Render();
}
