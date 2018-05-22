#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

#include "GameObject\Box.h"
#include "GameObject\Coin.h"

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

	vertice[0].color = 0xffcccccc;
	vertice[1].color = 0xffcccccc;
	vertice[2].color = 0xffcccccc;
	vertice[3].color = 0xffcccccc;

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

	deltaTime = 0.0f;

	for (int i = 0; i < MAX_SHADOW; i++) {
		shadows[i] = new Transform;
	}

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - vertice[0].position;

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	shadowCnt = 0;

	InitAnimation();

	isLeft = 0;
	status = STATUS_NORMAL;

	clips[status + isLeft]->Play();

	transform->SetWorldPosition(Vector2(-WINSIZE_X / 2 + 50, -17));
	transform->SetScale(Vector2(0.5f, 0.5f));

	currentBox.x = 1;
	currentBox.y = 0;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);

	SAFE_DELETE(transform);
	for (int i = 0; i < STATUS_END * 2; i++)
		SAFE_DELETE(clips[i]);

	for (int i = 0; i < MAX_SHADOW; i++)
		SAFE_DELETE(shadows[i]);

	SAFE_DELETE(collider);
}

void Rect::Update()
{
	//this->transform->DefaultControl2();
	this->DrawInterface();

	Vector2 min, max;
	Vector2 boxMin, boxMax;

	switch (status)
	{
	case STATUS_NORMAL:
		Jump();

		if (INPUT->GetKey(VK_LEFT)) {
			status = STATUS_ONLAND;
			isLeft = STATUS_END;
			clips[status + isLeft]->Play();
		}
		if (INPUT->GetKey(VK_RIGHT)) {
			status = STATUS_ONLAND;
			isLeft = 0;
			clips[status + isLeft]->Play();
		}

		break;
	case STATUS_ONLAND:
	{
		Move();
		Jump();

		this->collider->GetWorldAABBMinMax(this->transform, &min, &max);

		switch (currentBox.x)
		{
		case 1:
			box1[currentBox.y]->GetCollider()->GetWorldAABBMinMax(
				box1[currentBox.y]->GetTransform(), &boxMin, &boxMax);

			if (transform->GetWorldPosition().x < boxMin.x || 
			transform->GetWorldPosition().x > boxMax.x) {
				status = STATUS_TOLOW;
				clips[status + isLeft]->Play();
				vy = 0;
			}
			break;
		case 2:
			box2[currentBox.y]->GetCollider()->GetWorldAABBMinMax(
				box2[currentBox.y]->GetTransform(), &boxMin, &boxMax);

			if (max.x < boxMin.x ||
				min.x > boxMax.x) {
				status = STATUS_TOLOW;
				clips[status + isLeft]->Play();
				vy = 0;
			}
			break;
		case 3:
			box3[currentBox.y]->GetCollider()->GetWorldAABBMinMax(
				box3[currentBox.y]->GetTransform(), &boxMin, &boxMax);

			if (transform->GetWorldPosition().x < boxMin.x ||
				transform->GetWorldPosition().x > boxMax.x) {
				status = STATUS_TOLOW;
				clips[status + isLeft]->Play();
				vy = 0;
			}
			break;
		}

		if (INPUT->GetKeyUp(VK_LEFT)) {
			status = STATUS_NORMAL;
			isLeft = STATUS_END;
			clips[status + isLeft]->Play();
		}
		if (INPUT->GetKeyUp(VK_RIGHT)) {
			status = STATUS_NORMAL;
			isLeft = 0;
			clips[status + isLeft]->Play();
		}

		break;
	}
	case STATUS_TOUP:
	{
		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;

		float tempY = 25.0f;

		for (int i = 0; i < box1Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box1[i]->GetTransform(), box1[i]->GetCollider())) {
				box1[i]->GetCollider()->GetWorldAABBMinMax(box1[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x) {
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMax.y + (max.y - min.y) / 2 + tempY));
					vy = 0;
				}
			}
		}
		for (int i = 0; i < box2Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box2[i]->GetTransform(), box2[i]->GetCollider())) {
				box2[i]->GetCollider()->GetWorldAABBMinMax(box2[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x) {
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMax.y + (max.y - min.y) / 2 + tempY));
					vy = 0;
				}
			}
		}
		for (int i = 0; i < box3Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box3[i]->GetTransform(), box3[i]->GetCollider())) {
				box3[i]->GetCollider()->GetWorldAABBMinMax(box3[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x) {
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMax.y + (max.y - min.y) / 2 + tempY));
					vy = 0;
				}
			}
		}

		if (vy >= 0) {
			status = STATUS_TOLOW;
			clips[status + isLeft]->Play();
		}
		break;
	}
	case STATUS_TOLOW:
		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		
		//if (transform->GetWorldPosition().y >= -25.0f) {
		//	transform->SetWorldPosition(
		//		Vector2(transform->GetWorldPosition().x,
		//			-25.0f));
		//	status = STATUS_NORMAL;
		//	clips[status + isLeft]->Play();
		//}

		collider->GetWorldAABBMinMax(transform, &min, &max);

		for (int i = 0; i < box1Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box1[i]->GetTransform(), box1[i]->GetCollider())) {
				box1[i]->GetCollider()->GetWorldAABBMinMax(box1[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x &&
					transform->GetWorldPosition().y < boxMin.y) {
					currentBox.x = 1;
					currentBox.y = i;
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMin.y - (max.y - min.y) / 2));
					status = STATUS_NORMAL;
					clips[status + isLeft]->Play();
				}
			}
		}
		for (int i = 0; i < box2Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box2[i]->GetTransform(), box2[i]->GetCollider())) {
				box2[i]->GetCollider()->GetWorldAABBMinMax(box2[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x &&
					transform->GetWorldPosition().y < boxMin.y) {
					currentBox.x = 2;
					currentBox.y = i;
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMin.y - (max.y - min.y) / 2));
					status = STATUS_NORMAL;
					clips[status + isLeft]->Play();
				}
			}
		}
		for (int i = 0; i < box3Size; i++) {
			if (Collision::IsOverlap(transform, collider,
				box3[i]->GetTransform(), box3[i]->GetCollider())) {
				box3[i]->GetCollider()->GetWorldAABBMinMax(box3[i]->GetTransform(),
					&boxMin, &boxMax);
				if (transform->GetWorldPosition().x > boxMin.x &&
					transform->GetWorldPosition().x < boxMax.x &&
					transform->GetWorldPosition().y < boxMin.y) {
					currentBox.x = 3;
					currentBox.y = i;
					transform->SetWorldPosition(Vector2(
						transform->GetWorldPosition().x, boxMin.y - (max.y - min.y) / 2));
					status = STATUS_NORMAL;
					clips[status + isLeft]->Play();
				}
			}
		}
		break;
	}

	shadows[0]->Interpolate(this->shadows[0],
		transform, Frame::Get()->GetFrameDeltaSec() * 5.0f);

	for (int i = 1; i < MAX_SHADOW; i++) {
		shadows[i]->Interpolate(
			this->shadows[i],
			shadows[i - 1],
			// 이 값이 1에 가까우면 붙어서 움직임
			Frame::Get()->GetFrameDeltaSec() * 5.0f);
	}

	if (transform->GetWorldPosition().y > WINSIZE_Y / 2) {
		transform->SetWorldPosition(Vector2(-WINSIZE_X / 2 + 50, -17));
	}

	clips[status + isLeft]->Update(AniRepeatType_Loop);
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
			clips[status + isLeft]->GetCurrentData().maxFrame.x,
			clips[status + isLeft]->GetCurrentData().maxFrame.y,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			clips[status + isLeft]->GetCurrentData().currentFrame.x,
			clips[status + isLeft]->GetCurrentData().currentFrame.y,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");
	
	for (int i = 0; i < shadowCnt; i++) {
		this->pEffect->SetMatrix("matWorld", &shadows[i]->GetFinalMatrix().ToDXMatrix());
		this->RenderRect();
	}

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
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

void Rect::InitAnimation()
{
	AnimationData data;
	for (int i = 0; i < STATUS_END * 2; i++) {
		clips[i] = new AnimationClip;
	}

	data.maxFrame = Vector2(8.0f, 4.0f);
	//data.time = 5.0f;

	for (int i = 0; i < STATUS_END; i++) {
		switch (i)
		{
		case STATUS_NORMAL:
			data.keyName = L"Normal_Left_" + to_wstring(0);
			data.currentFrame = Vector2(4, 3);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"Normal_Right_" + to_wstring(0);
			data.currentFrame = Vector2(0, 3);
			clips[i]->PushAnimationData(data);
			break;
		case STATUS_ONLAND:
			data.keyName = L"ONLAND_Left_" + to_wstring(0);
			data.currentFrame = Vector2(4, 0);
			clips[i + STATUS_END]->PushAnimationData(data);
			data.keyName = L"ONLAND_Left_" + to_wstring(1);
			data.currentFrame = Vector2(5, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"ONLAND_Right_" + to_wstring(0);
			data.currentFrame = Vector2(0, 0);
			clips[i]->PushAnimationData(data);
			data.keyName = L"ONLAND_Right_" + to_wstring(1);
			data.currentFrame = Vector2(1, 0);
			clips[i]->PushAnimationData(data);

			break;
		case STATUS_TOUP:
			data.keyName = L"TOUP_Left_" + to_wstring(0);
			data.currentFrame = Vector2(6, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"TOUP_Right_" + to_wstring(0);
			data.currentFrame = Vector2(2, 0);
			clips[i]->PushAnimationData(data);

			break;
		case STATUS_TOLOW:
			data.keyName = L"TOUP_Left_" + to_wstring(0);
			data.currentFrame = Vector2(7, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"TOUP_Right_" + to_wstring(0);
			data.currentFrame = Vector2(3, 0);
			clips[i]->PushAnimationData(data);

			break;
		}

	}
	 
}

void Rect::Move() {
	float posX = transform->GetWorldPosition().x;

	Vector2 min, max;
	Vector2 boxMin, boxMax;

	if (INPUT->GetKey(VK_LEFT)) {
		if (posX - 25 > -WINSIZE_X / 2) {
			transform->MovePositionSelf(Vector2(-SPEED, 0));
			collider->GetWorldAABBMinMax(transform, &min, &max);

			for (int i = 0; i < box1Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box1[i]->GetTransform(), box1[i]->GetCollider())) {
					box1[i]->GetCollider()->GetWorldAABBMinMax(box1[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x > boxMax.x) {
						transform->SetWorldPosition(Vector2(
							boxMax.x + (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
			for (int i = 0; i < box2Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box2[i]->GetTransform(), box2[i]->GetCollider())) {
					box2[i]->GetCollider()->GetWorldAABBMinMax(box2[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x > boxMax.x) {
						transform->SetWorldPosition(Vector2(
							boxMax.x + (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
			for (int i = 0; i < box3Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box3[i]->GetTransform(), box3[i]->GetCollider())) {
					box3[i]->GetCollider()->GetWorldAABBMinMax(box3[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x > boxMax.x) {
						transform->SetWorldPosition(Vector2(
							boxMax.x + (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
		}

		isLeft = STATUS_END;
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		if (posX + 25 < WINSIZE_X / 2) {
			transform->MovePositionSelf(Vector2(SPEED, 0));
			collider->GetWorldAABBMinMax(transform, &min, &max);

			for (int i = 0; i < box1Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box1[i]->GetTransform(), box1[i]->GetCollider())) {
					box1[i]->GetCollider()->GetWorldAABBMinMax(box1[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x < boxMin.x) {
						transform->SetWorldPosition(Vector2(
							boxMin.x - (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
			for (int i = 0; i < box2Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box2[i]->GetTransform(), box2[i]->GetCollider())) {
					box2[i]->GetCollider()->GetWorldAABBMinMax(box2[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x < boxMin.x) {
						transform->SetWorldPosition(Vector2(
							boxMin.x - (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
			for (int i = 0; i < box3Size; i++) {
				if (Collision::IsOverlap(transform, collider,
					box3[i]->GetTransform(), box3[i]->GetCollider())) {
					box3[i]->GetCollider()->GetWorldAABBMinMax(box3[i]->GetTransform(),
						&boxMin, &boxMax);
					if (transform->GetWorldPosition().x < boxMin.x) {
						transform->SetWorldPosition(Vector2(
							boxMin.x - (max.x - min.x) / 2,
							transform->GetWorldPosition().y));
					}
				}
			}
		}

		isLeft = 0;
	}

	for (int i = 0; i < coinSize; i++) {
		if(!coin[i]->GetSelect() && 
			Collision::IsOverlap(transform, collider,
			coin[i]->GetTransform(), coin[i]->GetCollider())) {
			SOUND->Play("coin", 0.35f);
			coin[i]->SetSelect(true);
			shadowCnt++;
		}
	}
}

void Rect::Jump() {
	if (INPUT->GetKeyDown(VK_SPACE)) {
		SOUND->Play("jump", 0.30f);
		vy = JUMP;
		// 한번 점프만 하기 위해서 상태 바꾸고 거기서는 Jump() 함수 호출 안하게
		// 이중 점프하고 싶으면 거기서도 Jump() 호출하면됨
		status = STATUS_TOUP;

		clips[status + isLeft]->Play();
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

void Rect::WriteJsonData(wstring fileName, Json::Value * root)
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

void Rect::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}

void Rect::SetBox(Box ** box, int size, int num)
{
	switch (num)
	{
	case 1:
		box1 = box;
		box1Size = size;
		break;
	case 2:
		box2 = box;
		box2Size = size;
		break;
	case 3:
		box3 = box;
		box3Size = size;
		break;
	}
}