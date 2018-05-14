#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "GameObject\BaseBackGround.h"
#include "GameObject\MiniMap.h"
#include "./Common/Camera.h"

Program::Program()
{

	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("bg", "sounds/For River - Piano (Johnnys Version).mp3", true, true);

	mainCamera = new Camera;

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
	rect->Init();
	rect->SetCamera(mainCamera);
	rect->SetBgTransform(bg->GetTransform());

	baseBg = new BaseBackGround;
	baseBg->Init();

	miniMap = new MiniMap;
	miniMap->Init();

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	CreateRenderTargetTexture();

	miniMap->SetTexture(pRenderTexture);

	//SOUND->Play("Test");

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg4.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg1.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg2.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));

	currentBg = 0;
	bg->SetTexture(pTex[currentBg]);
	baseBg->SetTexture(pTex[currentBg]);
}

Program::~Program()
{
	rect->Release();
	bg->Release();
	baseBg->Release();
	miniMap->Release();
	SAFE_DELETE(rect);
	SAFE_DELETE(bg);
	SAFE_DELETE(baseBg);
	SAFE_DELETE(miniMap);
	SAFE_DELETE(mainCamera);

	for (int i = 0; i < 3; i++)
		SAFE_RELEASE(pTex[i]);
}

void Program::Update()
{
	SetCursor(NULL);
	ShowCursor(FALSE);

	if (INPUT->GetKeyDown(VK_SHIFT)) {
		if (!SOUND->IsPlaySound("bg"))
			SOUND->Play("bg", 0.5f);
	}

	currentBg = rect->GetCurrentBg();
	bg->SetTexture(pTex[currentBg]);
	baseBg->SetTexture(pTex[currentBg]);

	mainCamera->UpdateCamToDevice();
	bg->Update();
	baseBg->Update();
	rect->Update();
	miniMap->Update();
}

void Program::Render()
{
	bg->Render();
	rect->Render();

	// 후면버퍼에 데이터를 보낼 녀석에 대한 정보값
	LPDIRECT3DSURFACE9 pDeviceTargetSurface;
	// stencil이랑 depth 저장할 녀석
	LPDIRECT3DSURFACE9 pDeviceDepthAndStencilSuface;

	D2D::GetDevice()->GetRenderTarget(0, &pDeviceTargetSurface);
	D2D::GetDevice()->GetDepthStencilSurface(
		&pDeviceDepthAndStencilSuface);

	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->
		GetSurfaceLevel(0, &texSurface))) {
		D2D::GetDevice()->SetRenderTarget(0, texSurface);
		SAFE_RELEASE(texSurface);
	}

	D2D::GetDevice()->SetDepthStencilSurface(pRenderSurface);

	D2D::GetDevice()->Clear(
		0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xFF00FF00,
		1.0f, 0);

	// beginScene endScene Present 안써도됨 텍스처에 바로 그리는거니
	baseBg->Render();
	Vector2 pos = rect->GetTransform()->GetWorldPosition();
	rect->GetTransform()->SetWorldPosition(
		Vector2(pos.x / 2 - bg->GetTransform()->GetWorldPosition().x / 2, pos.y)
	);
	rect->Render();
	rect->GetTransform()->SetWorldPosition(pos);

	D2D::GetDevice()->SetRenderTarget(0, pDeviceTargetSurface);
	D2D::GetDevice()->SetDepthStencilSurface(pDeviceDepthAndStencilSuface);

	SAFE_RELEASE(pDeviceDepthAndStencilSuface);
	SAFE_RELEASE(pDeviceTargetSurface);

	//if (Input::Get()->GetKeyDown(VK_TAB)) {
	//	D3DXSaveTextureToFile(
	//		L"Test.png",
	//		D3DXIFF_PNG,
	//		pRenderTexture,
	//		NULL
	//	);
	//}

	miniMap->Render();
}

void Program::CreateRenderTargetTexture()
{
	int width = 1024;
	int height = 512;

	// 그냥 텍스처 변수 만드는거
	D2D::GetDevice()->CreateTexture(
		width, height,
		1,
		D3DUSAGE_RENDERTARGET, // 사용 용도
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pRenderTexture,
		NULL
	);

	// depth 와 stencil을 갖고 있는 surface 생성
	// 두 크기 동일해야됨 texture랑 surface
	D2D::GetDevice()->CreateDepthStencilSurface(
		width, height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,				// 버퍼를 교체할때 이전 교체 내용을 저장하지 않을 것이냐
		&pRenderSurface,
		NULL
	);
}
