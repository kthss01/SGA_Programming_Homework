#include "stdafx.h"
#include "Image.h"

Image::Image()
	:m_imageInfo(NULL)
	, fileName(NULL)
	, isTrans(false)
	, transColor(RGB(0, 0, 0))
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

HRESULT Image::Init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	if (m_imageInfo != NULL) Release();

	HDC hdc = GetDC(g_hWnd);

	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->loadType = LOAD_FILE;
	m_imageInfo->resID = 0;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);


	m_imageInfo->hBit =
		(HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height,
			LR_LOADFROMFILE);


	m_imageInfo->hOBit = (HBITMAP)SelectObject(
		m_imageInfo->hMemDC, m_imageInfo->hBit);
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	int len = strlen(fileName);
	// 길이값에 +1 한거는 null문자 때문에 한거
	this->fileName = new char[len + 1];
	strcpy_s(this->fileName, len + 1, fileName);

	this->isTrans = isTrans;
	this->transColor = transColor;

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
		SAFE_DELETE(fileName);

		isTrans = false;
		transColor = RGB(0, 0, 0);
	}
}

void Image::Render(HDC hdc) {

	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/
		GdiTransparentBlt
		(
			// 화면 부분
			hdc,					// 복사될 장소의 DC
			0,						// 복사될 좌표의 시작점 X
			0,						// 복사될 좌표의 시작점 Y
			m_imageInfo->width,		// 복사될 이미지의 가로 크기
			m_imageInfo->height,	// 복사될 이미지의 세로 크기

									// 이미지 부분
			m_imageInfo->hMemDC,	// 복사될 대상의 DC
									// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			0,						// 복사될 이미지 시작점 X
			0,						// 복사될 이미지 시작점 Y
									// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			m_imageInfo->width,		// 복사될 이미지 가로크기
			m_imageInfo->height,	// 복사될 이미지 세로크기
			transColor				// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		BitBlt(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::Render(HDC hdc, int destX, int destY)
{
	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		// transparent : 투명한
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/

		//// Test
		//HDC tempDC = CreateCompatibleDC(hdc);
		//HBITMAP tempBit = CreateCompatibleBitmap(hdc, m_imageInfo->width, m_imageInfo->height);
		//SelectObject(tempDC, tempBit);

		////PatBlt(tempDC, 0, 0, m_imageInfo->width, m_imageInfo->height, WHITENESS);

		//GdiTransparentBlt(
		//	tempDC, -1, 0, m_imageInfo->width+1, m_imageInfo->height+1,
		//	m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, transColor);


		////BitBlt(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
		////	tempDC, 0, 0, SRCCOPY);

		//GdiTransparentBlt(
		//	hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
		//	tempDC, 0, 0, m_imageInfo->width, m_imageInfo->height, RGB(254,0,254));

		//DeleteObject(tempBit);
		//DeleteDC(tempDC);

		GdiTransparentBlt
		(
			// 화면 부분
			hdc,					// 복사될 장소의 DC
			destX,					// 복사될 좌표의 시작점 X
			destY,					// 복사될 좌표의 시작점 Y
			m_imageInfo->width,		// 복사될 이미지의 가로 크기
			m_imageInfo->height,	// 복사될 이미지의 세로 크기

									// 이미지 부분
			m_imageInfo->hMemDC,	// 복사될 대상의 DC
									// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			0,						// 복사될 이미지 시작점 X
			0,						// 복사될 이미지 시작점 Y
									// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			m_imageInfo->width,		// 복사될 이미지 가로크기
			m_imageInfo->height,	// 복사될 이미지 세로크기
			transColor				// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		BitBlt(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void Image::Render(HDC hdc, int destX, int destY, int offsetX, int offsetY, int width, int height)
{
	GdiTransparentBlt
	(
		// 화면 부분
		hdc,					// 복사될 장소의 DC
		destX,					// 복사될 좌표의 시작점 X
		destY,					// 복사될 좌표의 시작점 Y
		width,					// 복사될 이미지의 가로 크기
		height,					// 복사될 이미지의 세로 크기

								// 이미지 부분
		m_imageInfo->hMemDC,	// 복사될 대상의 DC
								// 여기부분 수정하면 원하는 부분만 그릴 수 있음
		offsetX,				// 복사될 이미지 시작점 X
		offsetY,				// 복사될 이미지 시작점 Y
								// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
		width,					// 복사될 이미지 가로크기
		height,					// 복사될 이미지 세로크기
		transColor				// 복사할때 제외할 색상(기본적으로 마젠타 씀)
	);
}
