#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot)
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
									//L"./Shader/TextureMapping.fx",
									//L"./Shader/MultiTexture.fx",
		shaderFile.c_str(),
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

	vertice[0].position = Vector2(-50 + pivot.x, 50 + pivot.y);
	vertice[1].position = Vector2(-50 + pivot.x, -50 + pivot.y);
	vertice[2].position = Vector2(50 + pivot.x, -50 + pivot.y);
	vertice[3].position = Vector2(50 + pivot.x, 50 + pivot.y);

	vertice[0].color = 0x00ff0000;
	vertice[1].color = 0x00ffff00;
	vertice[2].color = 0x0000ff00;
	vertice[3].color = 0x000000ff;

	vertice[0].uv = Vector2(0, uv.y);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(uv.x, 0);
	vertice[3].uv = uv;

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

	pTex = NULL;

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - (vertice[0].position - pivot);

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	for (int i = 0; i < 2; i++) {
		tempTrans[i] = new Transform;
	}
	tempTrans[0]->MovePositionSelf(Vector2(-300, 0));
	tempTrans[1]->MovePositionSelf(Vector2(300, 0));

	showGizmo = true;
	showGizmoSpecial = false;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	//SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);

	for (int i = 0; i < 2; i++)
		SAFE_DELETE(tempTrans[i]);

	SAFE_DELETE(collider);
}

void Rect::Update()
{
	this->transform->DefaultControl2();

	for (int i = 0; i < 2; i++)
		tempTrans[i]->DefaultControl2();

	this->DrawInterface();

	if (INPUT->GetKeyDown('C')) {
		transform->Reset();
		for (int i = 0; i < 2; i++)
			tempTrans[i]->Reset();
		tempTrans[0]->MovePositionSelf(Vector2(-300, 0));
		tempTrans[1]->MovePositionSelf(Vector2(300, 0));
	}

	if (INPUT->GetKeyDown(VK_SPACE))
		showGizmo = !showGizmo;
	if (INPUT->GetKeyDown(VK_SHIFT))
		showGizmoSpecial = !showGizmoSpecial;
}

void Rect::Render()
{

	// 알파값 제거
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			1.0f,
			1.0f,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			0.0f,
			0.0f,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");
	
	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (showGizmo) {
		transform->RenderGizmo(true);
		tempTrans[0]->RenderGizmo(true);
		tempTrans[1]->RenderGizmo(true);

		collider->RenderGizmo(transform);
		collider->RenderGizmo(tempTrans[0]);
		collider->RenderGizmo(tempTrans[1]);
	}

	if (showGizmoSpecial) {
		collider->RenderGizmoSpecialRed(tempTrans[0]);
		collider->RenderGizmoSpecialGreen(transform);
		collider->RenderGizmoSpecialBlue(tempTrans[1]);
	}
}

void Rect::RenderRect()
{
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
				// 만약에 텍스처 렌더하는 방식이면 pEffect->setTexture로
				//D2D::GetDevice()->SetTexture(0, pTex);

				D2D::GetDevice()->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, 4, 0, 2);
			}
			this->pEffect->EndPass();
		}
	}

	this->pEffect->End();
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