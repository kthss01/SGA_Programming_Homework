#include "stdafx.h"
#include "Window.h"
#include "GameNode.h"

#include "IsoMap.h"

POINT Window::ptMouse = POINT{ 0,0 };
CTRL Window::_currentCTRL = CTRL_DRAW;

Window::Window()
{
	m_backBuffer = new Image();
	m_backBuffer->Init(SUBWINSIZEX, SUBWINSIZEY);
}


Window::~Window()
{
	SAFE_DELETE(m_backBuffer);
}

void Window::Init()
{
	CreateSubWindow();

	isActive = false;

	int tempX = 10;

	_btnDraw = CreateWindow("button", "Tile",
		// 자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		// 클릭했을때 어떤 값을 반환할지 hMenu
		tempX, 0, 100, 20, hWnd, HMENU(0), g_hInstance, NULL);
	_btnEraser = CreateWindow("button", "Eraser",
		// 자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		// 클릭했을때 어떤 값을 반환할지 hMenu
		tempX, 30, 100, 20, hWnd, HMENU(1), g_hInstance, NULL);

	_btnEraser = CreateWindow("button", "Init",
		// 자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		// 클릭했을때 어떤 값을 반환할지 hMenu
		tempX, 60, 100, 20, hWnd, HMENU(2), g_hInstance, NULL);

	_btnSave = CreateWindow("button", "Save",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 0, 100, 20, hWnd, HMENU(3), g_hInstance, NULL);

	_btnLoad = CreateWindow("button", "Load",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 30, 100, 20, hWnd, HMENU(4), g_hInstance, NULL);

	_btnStart = CreateWindow("button", "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 60, 100, 20, hWnd, HMENU(8), g_hInstance, NULL);

	_btnN1 = CreateWindow("button", "N1",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 35, 135, 35, 25, hWnd, HMENU(5), g_hInstance, NULL);

	_btnN2 = CreateWindow("button", "N2",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 35 + 60, 135, 35, 25, hWnd, HMENU(6), g_hInstance, NULL);

	_btnN3 = CreateWindow("button", "N3",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 35 + 120, 135, 35, 25, hWnd, HMENU(7), g_hInstance, NULL);



	clickFrame = { 0,0 };
	clickIndex = 0;
}

void Window::Release()
{

}

void Window::Update()
{

	if (currentScene != NULL)
	{
		currentScene->Update();
	}
}

void Window::Render()
{
	HDC hdc = GetDC(hWnd);
	PatBlt(m_backBuffer->GetMemDC(), 0, 0, SUBWINSIZEX, SUBWINSIZEY, WHITENESS);
	//==============================
	if (currentScene != NULL)
	{
		currentScene->Render(m_backBuffer->GetMemDC());
	}
		
	//=============================
	m_backBuffer->Render(hdc);
	ReleaseDC(hWnd, hdc);
}

void Window::SetScene(GameNode * scene)
{
	currentScene = scene;
	currentScene->Init();
}

LRESULT Window::WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_MOUSEMOVE:
		SUBWIN->SetIsActive(true);
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		break;
	case WM_PAINT:
	{
		//HDC hdc = GetDC(hWnd);
		//IMAGE->FindImage("sub_bg")->Render(hdc);
		//ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_COMMAND:
		// 프로그램 실행 중 사용자가 메뉴 항목을 선택하면 발생하는 메세지
		switch (LOWORD(wParam))
		{
		default:
			//// 클릭했을 때 0,1,2 값 중 하나가 들어옴 
			//_currentCTRL = (CTRL)(LOWORD(wParam));
			switch (LOWORD(wParam)) {
			case CTRL_DRAW:
			case CTRL_ERASER:
				_currentCTRL = (CTRL)(LOWORD(wParam));
				break;
			case CTRL_INIT:
				SUBWIN->GetIsoMap()->Init();
				break;
			case CTRL_SAVE:
				SUBWIN->GetIsoMap()->Save();
				break;
			case CTRL_LOAD:
				SUBWIN->GetIsoMap()->Load();
				break;
			case CTRL_NUM1:
			case CTRL_NUM2:
			case CTRL_NUM3:
				SUBWIN->SetFrameIndex(LOWORD(wParam) - 5);
				break;
			case CTRL_START:
				SCENE->ChangeScene("PokemonWorld");
				break;
			}
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::CreateSubWindow()
{
	// 로그 윈도우 생성
	int x, y, cx, cy;
	WNDCLASS wc;
	RECT rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)Window::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "sub";

	RegisterClass(&wc);

	// 부모 윈도우 오른쪽에 위치하게
	RECT rcWin;
	GetWindowRect(g_hWnd, &rcWin);

	cx = SUBWINSIZEX;
	cy = SUBWINSIZEY;
	x = rcWin.right;
	y = rcWin.top;

	rc.left = 0;
	rc.top = 0;
	rc.right = cx;
	rc.bottom = cy;

	HWND		hParenthWnd = NULL;
	HINSTANCE	hInst = NULL;

	hParenthWnd = g_hWnd;
	hInst = GetModuleHandle(NULL);

	hWnd = CreateWindow(
		"sub",
		"sub",
		WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, cx, cy, hParenthWnd, NULL,
		hInst, NULL);

	AdjustWindowRect(&rc, WINSTYLE, FALSE);

	SetWindowPos(hWnd, NULL, x, y,
		(rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
}