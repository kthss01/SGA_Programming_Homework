#include "stdafx.h"
#include "MiniMap.h"


MiniMap::MiniMap()
{
}


MiniMap::~MiniMap()
{
}

void MiniMap::Init()
{
	vertice[0].position = Vector2(-WINSIZE_X / 2 / 4 , WINSIZE_Y / 2 / 4);
	vertice[1].position = Vector2(-WINSIZE_X / 2 / 4, -WINSIZE_Y / 2 / 4);
	vertice[2].position = Vector2(WINSIZE_X / 2 / 4, -WINSIZE_Y / 2 / 4);
	vertice[3].position = Vector2(WINSIZE_X / 2 / 4, WINSIZE_Y / 2 / 4);

	vertice[0].uv = Vector2(0.0f, 1.0f);	// left bottom
	vertice[1].uv = Vector2(0.0f, 0.0f);	// left top
	vertice[2].uv = Vector2(1.0f, 0.0f); // right top
	vertice[3].uv = Vector2(1.0f, 1.0f); // right bottom

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

	transform->SetWorldPosition(
		Vector2(WINSIZE_X / 4 + WINSIZE_X / 8, -WINSIZE_Y / 4 - WINSIZE_Y / 8));
}

void MiniMap::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_DELETE(transform);
}

void MiniMap::Update()
{
	Vertex * pVertex = NULL;
	HRESULT hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	this->transform->DefaultControl2();
}

void MiniMap::Render()
{
	D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	D2D::GetDevice()->SetIndices(ib);
	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	D2D::GetDevice()->SetFVF(FVF);
	D2D::GetDevice()->SetTexture(0, pTex);

	D2D::GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 4, 0, 2);
}
