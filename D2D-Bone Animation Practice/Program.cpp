#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("bg", "sounds/Most Memorable OSTs Gallant Char.mp3",
		false, false);

	mainCamera = new Camera;

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
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

	SAFE_DELETE(bg);
	SAFE_DELETE(rect);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();
	bg->Update();
	rect->Update();

}

void Program::Render()
{
	bg->Render();
	rect->Render();
}
