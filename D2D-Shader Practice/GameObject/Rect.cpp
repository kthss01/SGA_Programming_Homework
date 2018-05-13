#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init()
{
	// 쉐이더 초기화
	// 에러값 확인이 어려우므로 
	// 쉐이더 로딩시 에러가 나면 에러 내용을 받아올 꺼
	LPD3DXBUFFER pError = NULL;

	// 쉐이더 옵션 지정
	DWORD shaderFlag = 0;
	// 지정되어야만 디버깅시 쉐이더 확인이 가능함
#ifdef _DEBUG
	shaderFlag |= D3DXSHADER_DEBUG;
#endif // _DEBUG

	// 쉐이더 내부에서 쓸 수 있는 게 #define, #include 
	D3DXCreateEffectFromFile(
		D2D::GetDevice(),			// 디바이스
									//L"./Shader/BaseColor.fx",	// 셰이더 파일
		L"./Shader/ColorTexture.fx",
		NULL,						// 셰이더 컴파일시 추가 #define
		NULL,						// 셰이더 컴파일시 추가 #include
									// include를 쓸 수 있는거
									// 외부에서 내부로 추가 할 수도 있음
									// 잘쓰진 않음
		shaderFlag,					// 셰이더 옵션
		NULL, // 셰이더 매개변수를 공유할 메모리풀
		&pEffect,
		&pError
	);

	if (pError != NULL) {
		string lastError = (char*)pError->GetBufferPointer();

		MessageBox(NULL, String::StringToWString(lastError).c_str(),
			L"Shader Error", MB_OK);
		SAFE_RELEASE(pError);
	}

	vertice[0].position = Vector2(-50, 50);
	vertice[1].position = Vector2(-50, -50);
	vertice[2].position = Vector2(50, -50);
	vertice[3].position = Vector2(50, 50);

	//vertice[0].color = 0xff808080;
	//vertice[1].color = 0xff808080;
	//vertice[2].color = 0xff808080;
	//vertice[3].color = 0xff808080;

	vertice[0].color = 0xff000000;
	vertice[1].color = 0xff000000;
	vertice[2].color = 0xff000000;
	vertice[3].color = 0xff000000;

	vertice[0].uv = Vector2(0, 1.0f / 4.0f);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(1.0f / 8.0f, 0);
	vertice[3].uv = Vector2(1.0f / 8.0f, 1.0f / 4.0f);

	//vertice[0].uv = Vector2(0, 1);
	//vertice[1].uv = Vector2(0, 0);
	//vertice[2].uv = Vector2(1, 0);
	//vertice[3].uv = Vector2(1, 1);

	stride = sizeof(Vertex);
	//FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	HRESULT hr = D2D::GetDevice()->CreateVertexBuffer(
		stride * 4,
		D3DUSAGE_WRITEONLY,		// dynamic 쓰게 되면
		FVF,
		D3DPOOL_MANAGED,		// 이걸로 해줘야함 default 해주면 data 남아있지 않음
		&vb,
		NULL
	);
	assert(SUCCEEDED(hr));

	Vertex * pVertex = NULL;
	hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	DWORD indices[] = {
		0,1,2,
		0,2,3,
	};

	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&ib,
		NULL
	);
	assert(SUCCEEDED(hr));

	void* pIndex;
	hr = ib->Lock(0, 0, &pIndex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pIndex, indices, sizeof(DWORD) * 6);
	hr = ib->Unlock();
	assert(SUCCEEDED(hr));

	transform = new Transform;
	
	transform->UpdateTransform();

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/mario_all.png",
		//L"Textures/Box.png",
		&pTex
	);
	assert(SUCCEEDED(hr));

	isLeft = false;
	status = STATUS_NORMAL;

	transform->SetWorldPosition(Vector2(0, WINSIZE_Y / 2 - 25 - 50));
	transform->SetScale(Vector2(0.5f, 0.5f));

	currentBg = 0;

	isSee = false;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);
}

void Rect::Update()
{
	if (INPUT->GetKeyDown(VK_UP)) {
		isSee = !isSee;
	}

	switch (status)
	{
	case STATUS_NORMAL:
		Jump();

		if (isSee) {
			currentIndexX = 3;
			currentIndexY = 3;
		}
		else if (isLeft) {
			currentIndexX = 4;
			currentIndexY = 3;
		}
		else {
			currentIndexX = 0;
			currentIndexY = 3;
		}

		if (INPUT->GetKey(VK_LEFT)) {
			isSee = false;
			status = STATUS_ONLAND;
			isLeft = true;
			currentIndexX = 4;
			currentIndexY = 0;
		}
		if (INPUT->GetKey(VK_RIGHT)) {
			isSee = false;
			status = STATUS_ONLAND;
			isLeft = false;
			currentIndexX = 0;
			currentIndexY = 0;
		}

		break;
	case STATUS_ONLAND:
		Move();
		Jump();

		if (INPUT->GetKeyUp(VK_LEFT)) {
			status = STATUS_NORMAL;
			isLeft = true;
			currentIndexX = 4;
			currentIndexY = 3;
		}
		if (INPUT->GetKeyUp(VK_RIGHT)) {
			status = STATUS_NORMAL;
			isLeft = false;
			currentIndexX = 0;
			currentIndexY = 3;
		}
		
		break;
	case STATUS_TOUP:
		if (isLeft) {
			currentIndexX = 6;
			currentIndexY = 0;
		}
		else {
			currentIndexX = 2;
			currentIndexY = 0;
		}

		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		if (vy >= 0) {
			status = STATUS_TOLOW;
			if (isLeft) {
				currentIndexX = 7;
				currentIndexY = 0;
			}
			else {				
				currentIndexX = 3;
				currentIndexY = 0;
			}
		}
		break;
	case STATUS_TOLOW:
		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		if (transform->GetWorldPosition().y >= bottom.y) {
			transform->SetWorldPosition(
				Vector2(transform->GetWorldPosition().x,
				bottom.y));
			status = STATUS_NORMAL;
		}
		break;
	}
	

	float tempX = (int)currentIndexX / 8.0f;
	float tempY = (int)currentIndexY / 4.0f;
	vertice[0].uv = Vector2(tempX, tempY + 1.0f/4.0f);
	vertice[1].uv = Vector2(tempX, tempY);
	vertice[2].uv = Vector2(tempX + 1.0f/8.0f, tempY);
	vertice[3].uv = Vector2(tempX + 1.0f/8.0f, tempY + 1.0f/4.0f);

	Vertex * pVertex = NULL;
	HRESULT hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	this->transform->DefaultControl2();
	this->DrawInterface();
}

void Rect::Render()
{
	// 알파값 제거
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	//D2D::GetDevice()->SetIndices(ib);
	//D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	//D2D::GetDevice()->SetFVF(FVF);
	//D2D::GetDevice()->SetTexture(0, pTex);
	//D2D::GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0, 0, 4, 0, 2);

	
	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");

	// 셰이더로 렌더
	UINT iPassNum = 0;

	this->pEffect->Begin(
		&iPassNum,			// pEffect에 있는 패스 수를 받아온다.
		0					// 플래그 
	);
	{
		for (UINT i = 0; i < iPassNum; i++) {
			this->pEffect->BeginPass(i);
			{
				D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
				D2D::GetDevice()->SetIndices(ib);
				D2D::GetDevice()->SetFVF(FVF);

				D2D::GetDevice()->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, 4, 0, 2);
			}
			this->pEffect->EndPass();
		}
	}

	this->pEffect->End();
	
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void Rect::Move() {
	float posX = transform->GetWorldPosition().x;
	
	if (INPUT->GetKey(VK_LEFT)) {
		//transform->MovePositionSelf(Vector2(-SPEED, 0));

		if (posX - 25 > -WINSIZE_X / 2) {
			if (posX >= WINSIZE_X / 4)
				transform->MovePositionSelf(Vector2(-SPEED, 0));
			else if (bgTransform->GetWorldPosition().x <
				WINSIZE_X / 2)
				bgTransform->MovePositionSelf(Vector2(SPEED, 0));
			else
				transform->MovePositionSelf(Vector2(-SPEED, 0));
		}
		else {
			switch (currentBg)
			{
			case 0:
				break;
			case 1:
				currentBg = 0;
				transform->SetWorldPosition(
					Vector2(WINSIZE_X / 2 - 25,
						transform->GetWorldPosition().y));
				bgTransform->SetWorldPosition(Vector2(-WINSIZE_X / 2, 0));
				break;
			case 2:
				currentBg = 1;
				transform->SetWorldPosition(
					Vector2(WINSIZE_X / 2 - 25,
						transform->GetWorldPosition().y));
				bgTransform->SetWorldPosition(Vector2(-WINSIZE_X / 2, 0));
				break;
			}
		}
		
		isLeft = true;

		// 뛰고있는 상태인 경우
		if (status == STATUS_ONLAND) {
			currentIndexX += 0.125f;
			if (currentIndexX >= 6) {
				currentIndexX = 4;
			}
		}
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		//transform->MovePositionSelf(Vector2(SPEED, 0));

		if (posX + 25 < WINSIZE_X / 2) {
			if (posX <= -WINSIZE_X / 4)
				transform->MovePositionSelf(Vector2(SPEED, 0));
			else if (bgTransform->GetWorldPosition().x >
				-WINSIZE_X / 2)
				bgTransform->MovePositionSelf(Vector2(-SPEED, 0));
			else
				transform->MovePositionSelf(Vector2(SPEED, 0));
		}
		else {
			switch (currentBg)
			{
			case 0:
				currentBg = 1;
				transform->SetWorldPosition(
					Vector2(-WINSIZE_X / 2 + 25,
						transform->GetWorldPosition().y));
				bgTransform->SetWorldPosition(Vector2(WINSIZE_X / 2, 0));
				break;
			case 1:
				currentBg = 2;
				transform->SetWorldPosition(
					Vector2(-WINSIZE_X / 2 + 25,
						transform->GetWorldPosition().y));
				bgTransform->SetWorldPosition(Vector2(WINSIZE_X / 2, 0));
				break;
			case 2:
				break;
			}
		}

		isLeft = false;

		// 뛰고있는 상태인 경우
		if (status == STATUS_ONLAND) {
			currentIndexX += 0.125f;
			if (currentIndexX >= 2) {
				currentIndexX = 0;
			}
		}
	}
}

void Rect::Jump() {
	if (INPUT->GetKeyDown(VK_SPACE)) {
		if (isLeft) {
			currentIndexX = 6;
			currentIndexY = 0;
		}
		else {
			currentIndexX = 2;
			currentIndexY = 0;
		}
		vy = JUMP;
		// 한번 점프만 하기 위해서 상태 바꾸고 거기서는 Jump() 함수 호출 안하게
		// 이중 점프하고 싶으면 거기서도 Jump() 호출하면됨
		status = STATUS_TOUP;
		bottom = transform->GetWorldPosition();
	}
}

void Rect::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Interface");
	{
		transform->DrawInterface();
	}
	ImGui::End();
#endif // IMGUI_USE
}
