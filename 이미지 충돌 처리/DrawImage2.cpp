#include "DrawImage2.h"



DrawImage2::DrawImage2()
{
	GdiplusStartupInput gdiplusStartupInput;
	gdiplusToken = NULL;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	pGraphics = new Graphics
	(GetDC(GetConsoleWindow()));
}


DrawImage2::~DrawImage2()
{
	for (int i = 0; i < images.size(); i++)
		delete images[i].image;
	images.clear();
	delete pGraphics;
	GdiplusShutdown(gdiplusToken);
}

void DrawImage2::Clear(int r, int g, int b)
{
	pGraphics->Clear(Color(r, g, b));
}

void DrawImage2::AddImage(WCHAR * fileName, Gdiplus::Rect rect)
{
	tagImageInfo imageInfo;
	imageInfo.image = new Image(fileName);
	imageInfo.rect = rect;

	images.push_back(imageInfo);
}

void DrawImage2::Drawing(int number)
{
	if (number > images.size()) return;

	pGraphics->DrawImage(images[number].image, images[number].rect);
}

void DrawImage2::Drawing(int number, Rect rect)
{
	if (number > images.size()) return;

	pGraphics->DrawImage(images[number].image, rect);
}
