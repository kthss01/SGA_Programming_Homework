#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <conio.h>

#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

class DrawImage
{
private:
	Gdiplus::Image* image;
	Gdiplus::Graphics* pGraphics;
	ULONG_PTR gdiplusToken;
public:
	DrawImage();
	~DrawImage();

	void Clear(int r, int g, int b);
	void AddImage(WCHAR* fileName);
	void Drawing(int x, int y);
};

