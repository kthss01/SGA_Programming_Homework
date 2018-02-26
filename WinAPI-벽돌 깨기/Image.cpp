#include "stdafx.h"
#include "Image.h"

Image::Image()
	:m_imageInfo(NULL)
{
}

Image::~Image() {
	this->Release();
}

HRESULT Image::Init(int width, int height) {
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_EMPTY;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	m_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	if (m_imageInfo->hBit == 0) {
		Release();

		return E_FAIL;
	}

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

void Image::Release() {
	if (m_imageInfo) {
		SelectObject(m_imageInfo->hMemDC, m_imageInfo->hOBit);
		DeleteObject(m_imageInfo->hOBit);
		DeleteDC(m_imageInfo->hMemDC);

		SAFE_DELETE(m_imageInfo);
	}
}

void Image::Render(HDC hdc) {
	BitBlt(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
		m_imageInfo->hMemDC, 0, 0, SRCCOPY);
}