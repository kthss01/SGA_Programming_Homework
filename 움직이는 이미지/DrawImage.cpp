#include "DrawImage.h"



DrawImage::DrawImage()
{
	GdiplusStartupInput gdiplusStartupInput;
	gdiplusToken = NULL;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	pGraphics = new Graphics
	(GetDC(GetConsoleWindow()));
}


DrawImage::~DrawImage()
{
	delete image;
	delete pGraphics;
	GdiplusShutdown(gdiplusToken);
}

void DrawImage::Clear(int r, int g, int b)
{
	pGraphics->Clear(Color(r, g, b));
}

void DrawImage::AddImage(WCHAR * fileName)
{
	image = new Image(fileName);
}

void DrawImage::Drawing(int x, int y)
{
	pGraphics->DrawImage(image, x, y);
}

