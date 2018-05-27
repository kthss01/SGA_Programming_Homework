#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

	mainCamera = new Camera;

	HRESULT hr;

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/yellow_panel.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/blue_panel.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/green_panel.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/grey_panel.png",
		&pTex[3]
	);
	assert(SUCCEEDED(hr));
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/red_panel.png",
		&pTex[4]
	);
	assert(SUCCEEDED(hr));

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	//SOUND->Play("Test");

	bottom = new Rect;
	bottom->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	bottom->SetTexture(pTex[Util::GetInt(5)]);
	bottom->SetCamera(mainCamera);
	bottom->GetTransform()->SetWorldPosition(Vector2(0, WINSIZE_Y / 2 + 50));
	bottom->GetTransform()->SetScale(Vector2(12.0f, 1.0f));

	root = new Json::Value();
	readJson = new Json::Value();
	float time = FRAME->GetNowRealTimeSec();
	Json::SetValue(*root, "test", time);

	WriteJsonData(L"Test.Json", root);
	ReadJsonData(L"Test.Json", readJson);

	float temp;
	Json::GetValue(*readJson, "test", temp);
}

Program::~Program()
{
	SAFE_RELEASE(bottom);
	
	for (int i = 0; i < 5; i++)
		SAFE_RELEASE(pTex[i]);

	SAFE_DELETE(bottom);
	SAFE_DELETE(mainCamera);

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	for (int i = 0; i < vRect.size(); i++) {
		vRect[i]->Release();
		SAFE_DELETE(vRect[i]);
	}
}

void Program::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE))
		CreateRect();

	mainCamera->UpdateCamToDevice();
	//rect->Update();

	for (int i = 0; i < vRect.size(); i++)
	{
		//vRect[i]->Update();
		if (vRect[i]->GetGravity()) {
			vRect[i]->GetTransform()->MovePositionWorld(Vector2(0, GRAVITY));
		}
		
		if (Collision::IsOverlap(
			bottom->GetTransform(), bottom->GetCollider(),
			vRect[i]->GetTransform(), vRect[i]->GetCollider())) {
			vRect[i]->SetGravity(false);
		}
	}

	for (int i = 0; i < vRect.size(); i++) {
		if (vRect[i]->GetGravity()) continue;

		for (int j = 0; j < vRect.size(); j++) {
			if (i == j) continue;
			if (Collision::IsOverlap(
				vRect[j]->GetTransform(), vRect[j]->GetCollider(),
				vRect[i]->GetTransform(), vRect[i]->GetCollider())) {
				vRect[j]->SetGravity(false);
			}
		}
	}
}

void Program::Render()
{
	//rect->Render();

	//Vector2 center = Vector2(0, 0);

	//GIZMO->Circle(
	//	center, 100.0f, 0xFF808000
	//);

	for (int i = 0; i < vRect.size(); i++)
		vRect[i]->Render();
}

void Program::CreateRect()
{
	Rect* temp = new Rect;
	temp->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	temp->SetTexture(pTex[Util::GetInt(5)]);
	temp->SetCamera(mainCamera);
	temp->SetGravity(true);
	temp->GetTransform()->SetScale(
		Vector2(Util::GetRandomSeed() * 1.75f + 0.25f,  
			Util::GetRandomSeed() * 1.75f + 0.25f));
	temp->GetTransform()->RotateSelf(
		Util::GetRandomSeed() * 2 * D3DX_PI);
	temp->GetTransform()->SetWorldPosition(
		Vector2(Util::GetFromInto(
			-WINSIZE_X / 2 + 100, WINSIZE_X / 2 - 100),
			Util::GetFromInto(
				-WINSIZE_Y / 2, -WINSIZE_Y / 2 - 100)));

	vRect.push_back(temp);
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