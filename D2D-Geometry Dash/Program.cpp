#include "stdafx.h"
#include "Program.h"

#include "GameObject\BackGround.h"
#include "GameObject\Rect.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("bg", "sounds/Forever Bound - Stereo Madness.mp3", true, true);

	mainCamera = new Camera;

	for (int i = 0; i < 2; i++) {
		rect[i] = new Rect;
		rect[i]->Init();
		rect[i]->SetCamera(mainCamera);

		bg[i] = new BackGround;
		bg[i]->Init();
		bottom[i] = new BackGround;
		bottom[i]->Init();
		bottom[i]->GetTransform()->MovePositionSelf(Vector2(0, WINSIZE_Y / 2 + 100));
	}
	bg[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X, 0));
	bottom[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X, 0));

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	//SOUND->Play("Test");
	//SOUND->Play("bg", 0.5f);

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));
	
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg2.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/rect.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/enemy.png",
		&pTex[3]
	);
	assert(SUCCEEDED(hr));

	for (int i = 0; i < 2; i++) {
		bg[i]->SetTexture(pTex[0]);
		bg[i]->SetCamera(mainCamera);
		bottom[i]->SetTexture(pTex[1]);
		bottom[i]->SetCamera(mainCamera);
		bg[i]->SetSpeed(1.0f);
	}
	rect[0]->SetTexture(pTex[2]);
	rect[0]->SetPlayer(true);

	rect[1]->SetTexture(pTex[3]);
	rect[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X / 2, 0));

	isPause = true;

	hr = D3DXCreateFont(
		D2D::GetDevice(),
		25,					// 너비
		0,					// 높이
		FW_BOLD,			// 두께, 두껍게 쓰겠다는거
		1,					// 이 둘은 크게 바꿀 꺼 없음
		FALSE,				// 이탤릭체 사용 여부
		DEFAULT_CHARSET,	// 이 둘은 크게 바꿀 꺼 없음
		OUT_DEFAULT_PRECIS,	// 외곽선 처리
		DEFAULT_QUALITY,	// 퀄리티
							// FF_DONTCARE 글자 크기 상관없이 큰 녀석 기준으로 잡아주는거 너비랑 높이 중에
		DEFAULT_PITCH || FF_DONTCARE,		// 옵션, 
		L"궁서",
		&font
	);
	assert(SUCCEEDED(hr));

	root = new Json::Value();
}

Program::~Program()
{
	for (int i = 0; i < 4; i++) {
		SAFE_RELEASE(pTex[i]);
	}

	for (int i = 0; i < 2; i++) {
		bg[i]->Release();
		SAFE_DELETE(bg[i]);
		bottom[i]->Release();
		SAFE_DELETE(bottom[i]);
		rect[i]->Release();
		SAFE_DELETE(rect[i]);
	}
	
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(root);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();

	if (INPUT->GetKeyDown('Z')) Init();
	if (INPUT->GetKeyDown('X')) Save();
	if (INPUT->GetKeyDown('C')) Load();

	if (INPUT->GetKeyDown('P')) isPause = !isPause;

	if (isPause) return;

	if (!SOUND->IsPlaySound("bg"))
		SOUND->Play("bg", 0.5f);

	score++;
	ChangeBGColor();

	for (int i = 0; i < 2; i++) {
		rect[i]->Update();
		bg[i]->Update();
		bottom[i]->Update();
	}
}

void Program::Render()
{
	// 이렇게하면 문제생김 왜인지는 모르겟음
	//for (int i = 0; i < 2; i++) {
	//	bg[i]->Render();
	//	bottom[i]->Render();
	//}

	for (int i = 0; i < 2; i++) {
		bg[i]->Render();
	}
	for (int i = 0; i < 2; i++) {
		bottom[i]->Render();
	}

	rect[0]->Render();

	for(int i=1; i<2; i++)
		rect[i]->Render();

	// RenderText
	RECT rc = { 10, 10, 0, 0 };
	wstring str;

	str = L"Score : ";
	str += to_wstring(score);

	// 멀티바이트면 DrawTextA
	font->DrawTextW(
		// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
		NULL,
		str.c_str(),
		-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
		&rc,
		// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
		// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
		DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
							 // 0x~~ 이거 귀찮으면 함수도 있음
							 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		0xFFFFFFFF
	);
}

void Program::Init()
{
	score = 0;
	rect[0]->GetTransform()->SetWorldPosition(Vector2(-75, 75));
	rect[0]->SetStatus(STATUS_IDLE);

	rect[1]->GetTransform()->SetWorldPosition(Vector2(WINSIZE_X / 2, 75));

	for (int i = 0; i < 2; i++) {
		bg[i]->SetColor(0xffffffff);
		bottom[i]->SetColor(0xffffffff);
	}
}

void Program::Save()
{
	int status = rect[0]->GetStatus();
	float vy = rect[0]->GetVy();
	float bottomY = rect[0]->GetBottomY();
	Vector2 pos = rect[0]->GetTransform()->GetWorldPosition();

	Vector2 pos2 = rect[1]->GetTransform()->GetWorldPosition();

	Json::SetValue(*root, "score", score);
	Json::SetValue(*root, "player_status", status);
	
	Json::SetValue(*root, "player_vy", vy);
	Json::SetValue(*root, "player_bottomY", bottomY);
	Json::SetValue(*root, "player_x", pos.x);
	Json::SetValue(*root, "player_y", pos.y);

	Json::SetValue(*root, "enemy_x", pos2.x);
	Json::SetValue(*root, "enemy_y", pos2.y);

	Json::SetValue(*root, "colorIndex", colorIndex);

	float color = bg[0]->GetColor();

	Json::SetValue(*root, "bg_color", color);

	WriteJsonData(L"./save/data.Json", root);
}

void Program::Load()
{
	int status;
	float vy;
	float bottomY;
	Vector2 pos;
	Vector2 pos2;

	ReadJsonData(L"./save/data.Json", root);
	Json::GetValue(*root, "score", score);
	Json::GetValue(*root, "player_status", status);

	Json::GetValue(*root, "player_vy", vy);
	Json::GetValue(*root, "player_bottomY", bottomY);
	Json::GetValue(*root, "player_x", pos.x);
	Json::GetValue(*root, "player_y", pos.y);

	Json::GetValue(*root, "enemy_x", pos2.x);
	Json::GetValue(*root, "enemy_y", pos2.y);

	Json::GetValue(*root, "colorIndex", colorIndex);

	float color;
	Json::GetValue(*root, "bg_color", color);
	for (int i = 0; i < 2; i++) {
		bg[i]->SetColor(color);
		bottom[i]->SetColor(color);
	}

	rect[0]->SetStatus((STATUS)status);
	rect[0]->SetVy(vy);
	rect[0]->SetBottomY(bottomY);
	rect[0]->GetTransform()->SetWorldPosition(pos);

	rect[1]->GetTransform()->SetWorldPosition(pos2);
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

void Program::ChangeBGColor()
{
	colorIndex = (score / 500) % 7;

	DWORD targetColor = scoreToColor[colorIndex];
	DWORD currentColor = bg[0]->GetColor();

	DWORD color;

	if (targetColor == currentColor)
		color = currentColor;
	else {
		color = currentColor;

		DWORD temp = targetColor ^ currentColor;
		byte r, g, b;
		r = temp;
		g = temp >> 8;
		b = temp >> 16;
		if (r != 0)
			color += 1;
		if (g != 0)
			color += 1 << 8;
		if (b != 0)
			color += 1 << 16;
	}

	for (int i = 0; i < 2; i++) {
		//bg[i]->SetColor(scoreToColor[colorIndex]);
		//bottom[i]->SetColor(scoreToColor[colorIndex]);
		bg[i]->SetColor(color);
		bottom[i]->SetColor(color);
	}
}
