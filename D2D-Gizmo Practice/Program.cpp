#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

	mainCamera = new Camera;

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		//L"Textures/mario_all.png",
		L"Textures/Box.png",
		&pTex
	);
	assert(SUCCEEDED(hr));

	rect = new Rect;
	rect->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	rect->SetTexture(pTex);
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
	rect->Release();
	
	SAFE_RELEASE(pTex);
	
	SAFE_DELETE(rect);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();
	rect->Update();
}

void Program::Render()
{
	rect->Render();
}
