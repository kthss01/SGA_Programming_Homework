#pragma once
class GameNode3
{
public:
	GameNode3();
	virtual ~GameNode3();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

