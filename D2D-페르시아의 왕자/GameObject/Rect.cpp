#include "stdafx.h"
#include "Rect.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init()
{
	vertice[0].position = Vector2(-20, 40);
	vertice[1].position = Vector2(-20, -40);
	vertice[2].position = Vector2(20, -40);
	vertice[3].position = Vector2(20, 40);

	// left
	vertice[0].uv = Vector2(0.0f, 1.0f);	// left bottom
	vertice[1].uv = Vector2(0.0f, 0.0f);	// left top
	vertice[2].uv = Vector2(1.0f / num[STATE_WALK], 0.0f); // right top
	vertice[3].uv = Vector2(1.0f / num[STATE_WALK], 1.0f); // right bottom

	// right
	//vertice[0].uv = Vector2(25.0f / IMG_WIDTH, 60.0f / IMG_HEIGHT);	// left bottom
	//vertice[1].uv = Vector2(25.0f / IMG_WIDTH, 0.0f);	// left top
	//vertice[2].uv = Vector2(0.0f, 0.0f); // right top
	//vertice[3].uv = Vector2(0.0f, 60.0f / IMG_HEIGHT); // right bottom

	stride = sizeof(Vertex);
	FVF = D3DFVF_XYZ | D3DFVF_TEX1;

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

	// bitmap 배경색 빼는 법
	//D3DCOLOR colorKey = D3DCOLOR_XRGB(186,254,202);
	//hr = D3DXCreateTextureFromFileEx(
	//	D2D::GetDevice(), L"Textures/prince.png", 
	//	D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
	//	D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
	//	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
	//	colorKey, NULL, NULL, &pTex
	//);

	LoadTexture();

	assert(SUCCEEDED(hr));

	deltaTime = 0.0f;
	currentIndex = 0.0f;
	isLeft = true;
	speed = 0.0f;
	currentState = STATE_IDLE;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_DELETE(transform);
}

void Rect::Update()
{
	deltaTime += Frame::Get()->GetFrameDeltaSec();

	//if (INPUT->GetKeyDown(VK_LEFT)) {
	//	if (isLeft == false || currentState != STATE_WALK) {
	//		currentIndex = 0;
	//		currentState = STATE_WALK;
	//	}
	//	isLeft = true;
	//	isIdle = false;
	//}
	//if (INPUT->GetKeyDown(VK_RIGHT)) {
	//	if (isLeft == true || currentState != STATE_WALK) {
	//		currentIndex = 0;
	//		currentState = STATE_WALK;
	//	}
	//	isLeft = false;
	//	isIdle = false;
	//}
	//if (INPUT->GetKey(VK_LEFT)) {
	//}
	//if (INPUT->GetKey(VK_RIGHT)) {
	//}
	//if (INPUT->GetKeyUp(VK_LEFT) || INPUT->GetKeyUp(VK_RIGHT)) {
	//	
	//}

	if (deltaTime > 0.15f) {
		deltaTime = 0.0f;

		//	if (currentIndex < num[currentState]) {
		//		currentIndex++;
		//		
		//	}
		//	else {
		//		if(isIdle)
		//			currentIndex = 0;
		//		else {
		//			switch (currentState)
		//			{
		//			case STATE_WALK:
		//				if (isLeft) {
		//					transform->MovePositionSelf(Vector2(-speed, 0.0f));
		//				}
		//				else {
		//					transform->MovePositionSelf(Vector2(speed, 0.0f));
		//				}
		//				currentIndex = 0;
		//				isIdle = true;
		//				break;
		//			case STATE_TURN:
		//				break;
		//			case STATE_RUN:
		//				break;
		//			case STATE_RUNTURN:
		//				break;
		//			case STATE_JUMP:
		//				break;
		//			case STATE_JUMP2:
		//				break;
		//			case STATE_RUNJUMP:
		//				break;
		//			case STATE_JUMPUP:
		//				break;
		//			case STATE_JUMPDOWN:
		//				break;
		//			case STATE_CLIMB:
		//				break;
		//			case STATE_DRINK:
		//				break;
		//			}
		//		}
		//	}

		switch (currentState)
		{
		case STATE_WALK:
			Move();
			break;
		case STATE_TURN:
			break;
		case STATE_RUN:
			Move();
			break;
		case STATE_RUNTURN:
			break;
		case STATE_JUMP:
			break;
		case STATE_RUNJUMP:
			break;
		case STATE_JUMPUP:
			currentIndex += 1.0f;
			if (currentIndex > 10 && vy < 0) {
				transform->MovePositionSelf(Vector2(0, vy));
				vy += GRAVITY;
				if (currentIndex > 12)
					currentIndex = 12;
			}
			if (vy >= 0) {
				if (transform->GetWorldPosition().y < land.y) {
					transform->MovePositionSelf(Vector2(0, vy));
					vy += GRAVITY;
					if (currentIndex > 14)
						currentIndex = 14;
				}
				else {
					transform->SetWorldPosition(land);
					if (currentIndex > num[currentState]) {
						currentIndex = 0;
						currentState = STATE_IDLE;
						transform->SetScale(Vector2(1.0f, 1.0f));
					}
				}
			}
			break;
		case STATE_JUMPDOWN:
			break;
		case STATE_CLIMB:
			break;
		case STATE_DRINK:
			break;
		case STATE_IDLE:
			Jump();

			if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey(VK_RIGHT)) {
				currentState = STATE_WALK;
				currentIndex = 0;
			}
			break;
		}
	
		if (currentState == STATE_IDLE) {
			float temp = (float)currentIndex / num[STATE_WALK];
			if (isLeft) {
				vertice[0].uv = Vector2(temp, 1.0f);
				vertice[1].uv = Vector2(temp, 0);
				vertice[2].uv = Vector2(temp + 1.0f / num[STATE_WALK], 0);
				vertice[3].uv = Vector2(temp + 1.0f / num[STATE_WALK], 1.0f);
			}
			else {
				vertice[0].uv = Vector2(temp + 1.0f / num[STATE_WALK], 1.0f);
				vertice[1].uv = Vector2(temp + 1.0f / num[STATE_WALK], 0);
				vertice[2].uv = Vector2(temp, 0);
				vertice[3].uv = Vector2(temp, 1.0f);
			}
		}
		else {
			float temp = (float)currentIndex / num[currentState];
			if (isLeft) {
				vertice[0].uv = Vector2(temp, 1.0f);
				vertice[1].uv = Vector2(temp, 0);
				vertice[2].uv = Vector2(temp + 1.0f / num[currentState], 0);
				vertice[3].uv = Vector2(temp + 1.0f / num[currentState], 1.0f);
			}
			else {
				vertice[0].uv = Vector2(temp + 1.0f / num[currentState], 1.0f);
				vertice[1].uv = Vector2(temp + 1.0f / num[currentState], 0);
				vertice[2].uv = Vector2(temp, 0);
				vertice[3].uv = Vector2(temp, 1.0f);
			}
		}
	}

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
	D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	D2D::GetDevice()->SetIndices(ib);
	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	D2D::GetDevice()->SetFVF(FVF);
	if (currentState == STATE_IDLE) {
		D2D::GetDevice()->SetTexture(0, pTex[STATE_WALK]);
	}
	else
		D2D::GetDevice()->SetTexture(0, pTex[currentState]);

	D2D::GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 4, 0, 2);
}

void Rect::LoadTexture()
{
	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/prince/walk_9x1.png",
		&pTex[STATE_WALK]
	);
	assert(SUCCEEDED(hr));
	num[STATE_WALK] = 9.0f;

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/prince/run_18x1.png",
		&pTex[STATE_RUN]
	);
	assert(SUCCEEDED(hr));
	num[STATE_RUN] = 18.0f;

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/prince/jumpup_17x1.png",
		&pTex[STATE_JUMPUP]
	);
	assert(SUCCEEDED(hr));
	num[STATE_JUMPUP] = 17.0f;
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

void Rect::Move()
{
	bool isMove = false;

	if (INPUT->GetKey(VK_LEFT)) {
		isMove = true;
		if (!isLeft)
			speed = 0;
		isLeft = true;
		speed -= 0.25f;
		if (fabs(speed) > 5.0f)
			speed = -5.0f;
		if (currentState == STATE_RUN) {
			currentIndex += 1.0f;
			if (currentIndex >= num[currentState]) {
				currentIndex = 11;
			}
			transform->MovePositionSelf(Vector2(speed, 0.0f));
		}
		if (fabs(speed) >= 1.0f && currentState == STATE_WALK) {
			currentState = STATE_RUN;
			currentIndex = 3;
		}
	}
	if (INPUT->GetKey(VK_RIGHT)) {
		isMove = true;
		if (isLeft)
			speed = 0;
		isLeft = false;
		speed += 0.25f;
		if (fabs(speed) > 5.0f)
			speed = 5.0f;
		if (currentState == STATE_RUN) {
			currentIndex += 1.0f;
			if (currentIndex >= num[currentState]) {
				currentIndex = 11;
			}
			transform->MovePositionSelf(Vector2(speed, 0.0f));
		}
		if (fabs(speed) > 1.0f && currentState == STATE_WALK) {
			currentState = STATE_RUN;
			currentIndex = 3;
		}
	}
	if (INPUT->GetKeyUp(VK_LEFT) || INPUT->GetKeyUp(VK_RIGHT)) {
		if (currentState == STATE_RUN) {
			currentIndex = 0;
		}
	}

	if (!isMove) {
		if (currentState == STATE_WALK) {
			currentIndex += 1.0f;
			if (currentIndex >= num[currentState]) {
				currentIndex = 0;
				currentState = STATE_IDLE;
				speed = 0;
				if (isLeft)
					transform->MovePositionSelf(Vector2(-1.0f, 0.0f));
				else
					transform->MovePositionSelf(Vector2(1.0f, 0.0f));
			}
		}
		if (currentState == STATE_RUN) {
			currentIndex += 1.0f;
			if (currentIndex > 2) {
				currentIndex = 0;
				currentState = STATE_IDLE;
			}
		}
	}
}

void Rect::Jump()
{
	if (INPUT->GetKeyDown(VK_UP)) {
		currentIndex = 0;
		vy = JUMP;
		currentState = STATE_JUMPUP;
		land = transform->GetWorldPosition();
		transform->SetScale(Vector2(0.8f, 1.25f));
	}
}
