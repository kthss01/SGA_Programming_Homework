#pragma once
class GameNode5
{
public:
	GameNode5();
	virtual ~GameNode5();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	LRESULT MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

