#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"

#include "./Common/Camera.h"
#include "./Common/Transform.h"

Program::Program()
{
	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("bgm", "sounds/클라 인 러브 - Linda 행진곡.mp3", true, true);

	mainCamera = new Camera;

	bg = new BackGround;
	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.jpg",
		&pTex2
	);
	assert(SUCCEEDED(hr));
	bg->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	bg->SetCamera(mainCamera);
	bg->SetTexture(pTex2);

	////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_Head.png",
		&pTex[BODYPART_HEAD]
	);
	assert(SUCCEEDED(hr));

	for (int i = 0; i < BODYPART_END; i++)
		bodyPart[i] = new Rect;

	bodyPart[BODYPART_HEAD]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-50, -50), Vector2(50, 50), Vector2(0,-50));
	bodyPart[BODYPART_HEAD]->SetTexture(pTex[BODYPART_HEAD]);
	bodyPart[BODYPART_HEAD]->SetCamera(mainCamera);

	////////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_Body.png",
		&pTex[BODYPART_BODY]
	);
	assert(SUCCEEDED(hr));

	bodyPart[BODYPART_BODY]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-50, -50), Vector2(50, 50));
	bodyPart[BODYPART_BODY]->SetTexture(pTex[BODYPART_BODY]);
	bodyPart[BODYPART_BODY]->SetCamera(mainCamera);
	////////////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_LeftArm.png",
		&pTex[BODYPART_LEFTARM]
	);
	assert(SUCCEEDED(hr));

	bodyPart[BODYPART_LEFTARM]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-25, -50), Vector2(25, 50), Vector2(-25, 0));
	bodyPart[BODYPART_LEFTARM]->SetTexture(pTex[BODYPART_LEFTARM]);
	bodyPart[BODYPART_LEFTARM]->SetCamera(mainCamera);

	//////////////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_RightArm.png",
		&pTex[BODYPART_RIGHTARM]
	);
	assert(SUCCEEDED(hr));

	bodyPart[BODYPART_RIGHTARM]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-25, -50), Vector2(25, 50), Vector2(25, 0));
	bodyPart[BODYPART_RIGHTARM]->SetTexture(pTex[BODYPART_RIGHTARM]);
	bodyPart[BODYPART_RIGHTARM]->SetCamera(mainCamera);

	/////////////////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_LeftLeg.png",
		&pTex[BODYPART_LEFTLEG]
	);
	assert(SUCCEEDED(hr));

	bodyPart[BODYPART_LEFTLEG]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-25, -50), Vector2(25, 50), Vector2(0, 50));
	bodyPart[BODYPART_LEFTLEG]->SetTexture(pTex[BODYPART_LEFTLEG]);
	bodyPart[BODYPART_LEFTLEG]->SetCamera(mainCamera);

	///////////////////////////////////////////////////////////
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/BodyParts_RightLeg.png",
		&pTex[BODYPART_RIGHTLEG]
	);
	assert(SUCCEEDED(hr));

	bodyPart[BODYPART_RIGHTLEG]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(-25, -50), Vector2(25, 50), Vector2(0, 50));
	bodyPart[BODYPART_RIGHTLEG]->SetTexture(pTex[BODYPART_RIGHTLEG]);
	bodyPart[BODYPART_RIGHTLEG]->SetCamera(mainCamera);

	////////////////////////////////////////////////////////////
	bodyPart[BODYPART_HEAD]->GetTransform()->MovePositionSelf(Vector2(0, -100));
	bodyPart[BODYPART_LEFTARM]->GetTransform()->MovePositionSelf(Vector2(-100, 0));
	bodyPart[BODYPART_RIGHTARM]->GetTransform()->MovePositionSelf(Vector2(100, 0));
	bodyPart[BODYPART_LEFTLEG]->GetTransform()->MovePositionSelf(Vector2(-50, 100));
	bodyPart[BODYPART_RIGHTLEG]->GetTransform()->MovePositionSelf(Vector2(50, 100));

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	//SOUND->Play("Test");

	currentPart = -1;
	
	root = new Json::Value();
	readJson = new Json::Value();

	aniStart = false;

	bottom = new Transform;
	bottom->MovePositionSelf(Vector2(0,150));
	bottom->AddChild(bodyPart[BODYPART_BODY]->GetTransform());
	bottom->AddChild(bodyPart[BODYPART_HEAD]->GetTransform());
	bodyPart[BODYPART_BODY]->GetTransform()->AddChild(bodyPart[BODYPART_LEFTARM]->GetTransform());
	bodyPart[BODYPART_BODY]->GetTransform()->AddChild(bodyPart[BODYPART_RIGHTARM]->GetTransform());
	bodyPart[BODYPART_BODY]->GetTransform()->AddChild(bodyPart[BODYPART_LEFTLEG]->GetTransform());
	bodyPart[BODYPART_BODY]->GetTransform()->AddChild(bodyPart[BODYPART_RIGHTLEG]->GetTransform());
}

Program::~Program()
{
	for (int i = 0; i < BODYPART_END; i++) {
		bodyPart[i]->Release();
		SAFE_DELETE(bodyPart[i]);
	}

	bg->Release();
	SAFE_DELETE(bg);

	for(int i=0; i<6; i++)
		SAFE_RELEASE(pTex[i]);
	SAFE_RELEASE(pTex2);
	
	SAFE_DELETE(mainCamera);

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	SAFE_DELETE(bottom);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();

	bg->Update();

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		currentPart = -1;
		for (int i = 0; i < BODYPART_END; i++) {
			if (bodyPart[i]->isMouseCollision()) {
				currentPart = i;
			}
		}
	}
	if (INPUT->GetKeyDown('P')) {
		for (int i = 0; i < BODYPART_END; i++)
			this->InitTransform(bodyPart[i]->GetTransform());

		bodyPart[BODYPART_HEAD]->GetTransform()->MovePositionSelf(Vector2(0, -100));
		bodyPart[BODYPART_LEFTARM]->GetTransform()->MovePositionSelf(Vector2(-100, 0));
		bodyPart[BODYPART_RIGHTARM]->GetTransform()->MovePositionSelf(Vector2(100, 0));
		bodyPart[BODYPART_LEFTLEG]->GetTransform()->MovePositionSelf(Vector2(-50, 100));
		bodyPart[BODYPART_RIGHTLEG]->GetTransform()->MovePositionSelf(Vector2(50, 100));
		bottom->Reset();
	}
	if (INPUT->GetKeyDown('I')) {
		for (int i = 0; i < BODYPART_END; i++)
			this->SaveTransform(
				bodyPart[i]->GetTransform(), L"Init_" + to_wstring(i));
		WriteJsonData(L"./saves/Init.Json", root);
	}
	if (INPUT->GetKeyDown('O')) {
		ReadJsonData(L"./saves/Init.Json", readJson);
		for (int i = 0; i < BODYPART_END; i++) {
			this->LoadTransform(
				bodyPart[i]->GetTransform(), L"Init_" + to_wstring(i));
		}
	}
	if (INPUT->GetKeyDown(VK_SPACE)) {
		Transform temp;
		vector<Transform> vTemp;
		for (int i = 0; i < BODYPART_END; i++) {
			Transform* part = bodyPart[i]->GetTransform();
			temp.SetScale(part->GetScale());
			//temp.SetWorldPosition(part->GetWorldPosition());
			temp.SetLocalPosition(part->GetLocalPosition());
			//D3DXQUATERNION quat = part->GetWorldRotateQuaternion();
			D3DXQUATERNION quat = part->GetLocalRotateQuaternion();
			temp.SetRotateLocal(quat);
			vTemp.push_back(temp);
		}
		vTrans.push_back(vTemp);
	}
	if (INPUT->GetKeyDown(VK_RETURN)) {
		aniStart = !aniStart;
		deltaTime = 0;
		currentAni = 0;
	}

	if (INPUT->GetKeyDown('C')) {
		if (!SOUND->IsPlaySound("bgm"))
			SOUND->Play("bgm");
	}

	if (INPUT->GetKeyDown('K')) { SaveAnimation(); }
	if (INPUT->GetKeyDown('L')) { LoadAnimation(); }
	if (INPUT->GetKeyDown('J')) { vTrans.clear(); }

	if (aniStart) {
		deltaTime += FRAME->GetFrameDeltaSec();
		if (deltaTime > 0.5f) {
			deltaTime = 0;
			if (currentAni < vTrans.size()) {
				for (int i = 0; i < BODYPART_END; i++) {
					bodyPart[i]->SetTransform(&vTrans[currentAni][i]);
				}
				currentAni++;
			}
			else {
				currentAni = 0;
			}
		}
	}

	for (int i = 0; i < BODYPART_END; i++) {
		bodyPart[i]->Update();
	}

	if (currentPart != -1)
		bodyPart[currentPart]->GetTransform()->DefaultControl2();
	else
		bottom->DefaultControl2();
}

void Program::Render()
{
	bg->Render();

	for (int i = BODYPART_END - 1; i >= 0; i--) {
		bodyPart[i]->Render();
	}
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

void Program::InitTransform(Transform * pTrans)
{
	pTrans->Reset();
}

void Program::SaveTransform(Transform * pTrans, wstring name)
{
	Vector2 scale = pTrans->GetScale();
	Vector2 pos = pTrans->GetLocalPosition();
	D3DXQUATERNION quat = pTrans->GetLocalRotateQuaternion();

	Json::SetValue(*root, String::WStringToString(name + L"_scale_x"), scale.x);
	Json::SetValue(*root, String::WStringToString(name + L"_scale_y"), scale.y);
	
	Json::SetValue(*root, String::WStringToString(name + L"_position_x"), pos.x);
	Json::SetValue(*root, String::WStringToString(name + L"_position_y"), pos.y);

	Json::SetValue(*root, String::WStringToString(name + L"_quaternion_x"), quat.x);
	Json::SetValue(*root, String::WStringToString(name + L"_quaternion_y"), quat.y);
	Json::SetValue(*root, String::WStringToString(name + L"_quaternion_z"), quat.z);
	Json::SetValue(*root, String::WStringToString(name + L"_quaternion_w"), quat.w);
}

void Program::LoadTransform(Transform * outTrans, wstring name)
{
	float scale[2];
	float pos[2];
	float quat[4];

	Json::GetValue(*readJson, String::WStringToString(name + L"_scale_x"), scale[0]);
	Json::GetValue(*readJson, String::WStringToString(name + L"_scale_y"), scale[1]);

	Json::GetValue(*readJson, String::WStringToString(name + L"_position_x"), pos[0]);
	Json::GetValue(*readJson, String::WStringToString(name + L"_position_y"), pos[1]);

	Json::GetValue(*readJson, String::WStringToString(name + L"_quaternion_x"), quat[0]);
	Json::GetValue(*readJson, String::WStringToString(name + L"_quaternion_y"), quat[1]);
	Json::GetValue(*readJson, String::WStringToString(name + L"_quaternion_z"), quat[2]);
	Json::GetValue(*readJson, String::WStringToString(name + L"_quaternion_w"), quat[3]);

	D3DXQUATERNION newQuat = D3DXQUATERNION(quat[0], quat[1], quat[2], quat[3]);

	outTrans->SetScale(Vector2(scale[0], scale[1]));
	outTrans->SetLocalPosition(Vector2(pos[0], pos[1]));
	outTrans->SetRotateLocal(newQuat);
}

void Program::SaveAnimation()
{
	aniSize = vTrans.size();
	float temp = (float)aniSize;
	Json::SetValue(*root, "AniSize", temp);

	for (int i = 0; i < aniSize; i++) {
		for (int j = 0; j < BODYPART_END; j++) {
			this->SaveTransform(
				&vTrans[i][j], L"Ani_" + to_wstring(i) + L"_" + to_wstring(j));
		}
	}
	WriteJsonData(L"./saves/Ani.Json", root);
}

void Program::LoadAnimation()
{
	ReadJsonData(L"./saves/Ani.Json", readJson);
	float temp;
	Json::GetValue(*readJson, "AniSize", temp);
	aniSize = (int)temp;

	vTrans.clear();

	for (int i = 0; i < aniSize; i++) {
		vector<Transform> vTemp;
		Transform tempTrans;
		for (int j = 0; j < BODYPART_END; j++) {
			this->LoadTransform(
				&tempTrans, L"Ani_" + to_wstring(i) + L"_" + to_wstring(j));
			vTemp.push_back(tempTrans);
		}
		vTrans.push_back(vTemp);
	}
}
