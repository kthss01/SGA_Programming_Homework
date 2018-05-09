#pragma once
class D2D
{
private:
	// 다이렉트 쓸 수 있는지 없는지 판단
	LPDIRECT3D9 d3d;	// 컴퓨터 사양을 가지고 올 녀석 그래픽 카드 정보
	// 실질적으로 개발할때 렌더할때 쓰는 녀석
	static LPDIRECT3DDEVICE9 device; // 항상 이 녀석을 이용해줘야함

	static D2D * instance;	

	D2D();
	~D2D();
public:
	static D2D * Get();
	static void Delete();

	static LPDIRECT3DDEVICE9 GetDevice() { return device; }

	void BeginScene(DWORD color);
	void EndScene();
};

