#pragma once
class Program;

// 창을 만드는 역할만 할꺼

class Window
{
public:
	Window();
	~Window();

	WPARAM Run();

	float color[4];
private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static Program* program;

	bool bFullScreen = false;
};
