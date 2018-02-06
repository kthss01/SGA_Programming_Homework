#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <conio.h>
#include <vector>

#pragma comment (lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

struct tagImageInfo {
	Gdiplus::Image* image;
	Gdiplus::Rect rect;
};

class DrawImage2
{
private:
	Gdiplus::Graphics* pGraphics;
	ULONG_PTR gdiplusToken;

	vector<tagImageInfo> images;
public:
	DrawImage2();
	~DrawImage2();

	void Clear(int r, int g, int b);
	void AddImage(WCHAR* fileName, Gdiplus::Rect rect);
	void Drawing(int number);
	void Drawing(int number, Rect rect);
};

