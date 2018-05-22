#include "stdafx.h"
#include "D2D.h"

D2D* D2D::instance = NULL;
LPDIRECT3DDEVICE9 D2D::device = NULL;

D2D::D2D()
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	// 안에 들어간 값 false면 error 값 출력
	assert(d3d != NULL);

	// 속성값 어떤식으로 direct 생성할지
	D3DPRESENT_PARAMETERS param = { 0 };
	// 창모드 true, 전체화면이면 모니터 사이즈를 알아야함
	// 이게 창크기 변경해도 창크기 잡아줌
	param.Windowed = TRUE;	// 창모드로 할지 전체화면으로 할지 false면 전체
	// swap effect (버퍼가 교체되는 순간) 
	// 전방 버퍼 백 버퍼로 화면 체인지 되는 게 swap chain effect
	// 메모리 관련 쪽으로 들어가 있음
	// DISCARD는 기본값 저장을 하지 않고 전방 버퍼로 넘겨줌
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// Depth 깊이값 Stencil 사람 모양 종이에 페인트 칠하면 찍어내는거
	// Stencil Direct에서도 고급기능 D2D에서는 쓰지 않을꺼
	// Depth를 16비트 2Byte형으로 쓰겠다는거
	// 참고
	// 좀 많이 들어가는건 
	// 이건 컬러값 지정할때 많이 쓰는거
	// D3DFMT_A32B32G32R32F 
	// 인자값 4개 들어가는데 하나당 4바이트로 들어가는 float이라는거
	param.AutoDepthStencilFormat = D3DFMT_D16;
	// Depth 값 Stencil 값 자동으로 사용하겠냐는거
	param.EnableAutoDepthStencil = TRUE;

	HRESULT hr;

	hr = d3d->CreateDevice(
		// 두 개의 모니터가 있게 되면 주 모니터에 띄우겠다
		// 참고로 더블 모니터 쓸 때 전체화면 안하는걸 추천
		// 모니터 크기 자동으로 계산해줄때 비율값이 안맞아서 화면 검은색으로 나옴
		D3DADAPTER_DEFAULT,	// 주 모니터를 사용하겠다
		D3DDEVTYPE_HAL,
		g_hWnd,
		// hardware vertex processing 
		// 정점에 대한 라이트 처리 누가 할지 지정하는거 하드웨어 / 소프트웨어
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&param,
		&device
	);

	// 에러가 나오고 어느 cpp 어떠한 위치에서 어떤 이유로 뜨면서 중단됨
	// 설명을 붙어 줄 수 있음
	// 안에 bool값만 들어갈 수 있는데 문자열은 무조건 true라서 && 연산하면됨
	assert(SUCCEEDED(hr) && "Not CreateDevice");

}


D2D::~D2D()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

// 싱글톤 처리
D2D * D2D::Get()
{
	if (instance == NULL)
		instance = new D2D;
	return instance;
}

void D2D::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void D2D::BeginScene(DWORD color)
{
	// Clear 화면 지워주는거, 
	// 2번째 인자는 지울 크기 NULL이면 화면 전체
	// CLEAR_TARGET 화면에 출력될 녀석
	// ZBUFFER clear 깊이값 초기화
	// 1.0f ZBUFFER 몇으로 초기화 할지
	// z 값은 0.1 에서 ~ 1 사이값 작은게 앞으로 나오는거
 	// 0.0f Stancil 몇으로 초기화 할지
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		color, 1.0f, 0.0f);

	device->BeginScene(); // 후면 버퍼에 쓴다.
}

void D2D::EndScene()
{
	device->EndScene();	// 렌더가 끝났다는거
	// present 하면 endScene 전까지의 내용이 들어가게됨
	device->Present(NULL, NULL, NULL, NULL); // 후면 -> 전면
}
