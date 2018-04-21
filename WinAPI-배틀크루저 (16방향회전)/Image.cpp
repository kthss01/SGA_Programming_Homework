#include "stdafx.h"
#include "Image.h"

// 알파 블랜드 사용을 위한 라이브러리(lib)
#pragma comment(lib, "msimg32.lib")

Image::Image()
	:m_imageInfo(NULL)
	, fileName(NULL)
	, isTrans(false)
	, transColor(RGB(0, 0, 0))
	, blendImage(NULL)
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

	// 기본값으로 초기화 해준다고 보면 됨
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend 옵션
	// 기타 옵션
	blendFunc.BlendFlags = 0;
	// 비트맵 이미지 32비트 인경우 AC_SRC_ALPHA
	// 그 외에 16비트 24비트 등등 나머지는 0으로 초기화
	blendFunc.AlphaFormat = 0;
	// 블랜딩 연산자 이것만 사용하면 된다 알고 있자
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// 렌더에서 쓰고있는 이미지는 알파값 줄 수 없음
	// 알파값 줄 수 있는 이미지는 마젠타 뺄 수 없음
	// 그래서 마젠타 빼고 알파값 주는 식으로 할꺼

	tempDC = CreateCompatibleDC(hdc);

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

	// 기본값으로 초기화 해준다고 보면 됨
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend 옵션
	// 기타 옵션
	blendFunc.BlendFlags = 0;
	// 비트맵 이미지 32비트 인경우 AC_SRC_ALPHA
	// 그 외에 16비트 24비트 등등 나머지는 0으로 초기화
	blendFunc.AlphaFormat = 0;
	// 블랜딩 연산자 이것만 사용하면 된다 알고 있자
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// 렌더에서 쓰고있는 이미지는 알파값 줄 수 없음
	// 알파값 줄 수 있는 이미지는 마젠타 뺄 수 없음
	// 그래서 마젠타 빼고 알파값 주는 식으로 할꺼

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
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

	m_imageInfo->x = x - (width / width / 2);
	m_imageInfo->y = y - (height / height / 2);

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

	// 기본값으로 초기화 해준다고 보면 됨
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend 옵션
	// 기타 옵션
	blendFunc.BlendFlags = 0;
	// 비트맵 이미지 32비트 인경우 AC_SRC_ALPHA
	// 그 외에 16비트 24비트 등등 나머지는 0으로 초기화
	blendFunc.AlphaFormat = 0;
	// 블랜딩 연산자 이것만 사용하면 된다 알고 있자
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// 렌더에서 쓰고있는 이미지는 알파값 줄 수 없음
	// 알파값 줄 수 있는 이미지는 마젠타 뺄 수 없음
	// 그래서 마젠타 빼고 알파값 주는 식으로 할꺼

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
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

	m_imageInfo->maxFrameX = frameX - 1;
	m_imageInfo->maxFrameY = frameY - 1;
	m_imageInfo->frameWidth = width / frameX;
	m_imageInfo->frameHeight = height / frameY;

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

	// 기본값으로 초기화 해준다고 보면 됨
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend 옵션
	// 기타 옵션
	blendFunc.BlendFlags = 0;
	// 비트맵 이미지 32비트 인경우 AC_SRC_ALPHA
	// 그 외에 16비트 24비트 등등 나머지는 0으로 초기화
	blendFunc.AlphaFormat = 0;
	// 블랜딩 연산자 이것만 사용하면 된다 알고 있자
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// 렌더에서 쓰고있는 이미지는 알파값 줄 수 없음
	// 알파값 줄 수 있는 이미지는 마젠타 뺄 수 없음
	// 그래서 마젠타 빼고 알파값 주는 식으로 할꺼

	tempDC = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
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

	// 프레임 렌더의 중심점이 x가 되게끔
	m_imageInfo->x = x - (width / frameX / 2);
	m_imageInfo->y = y - (height / frameY / 2);

	m_imageInfo->width = width;
	m_imageInfo->height = height;

	m_imageInfo->maxFrameX = frameX - 1;
	m_imageInfo->maxFrameY = frameY - 1;
	m_imageInfo->frameWidth = width / frameX;
	m_imageInfo->frameHeight = height / frameY;

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

	// 기본값으로 초기화 해준다고 보면 됨
	blendImage = new IMAGE_INFO;
	blendImage->loadType = LOAD_EMPTY;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);
	blendImage->hBit =
		(HBITMAP)CreateCompatibleBitmap(hdc,
			m_imageInfo->width, m_imageInfo->height);
	blendImage->hOBit =
		(HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZEX;
	blendImage->height = WINSIZEY;

	// alpha blend 옵션
	// 기타 옵션
	blendFunc.BlendFlags = 0;
	// 비트맵 이미지 32비트 인경우 AC_SRC_ALPHA
	// 그 외에 16비트 24비트 등등 나머지는 0으로 초기화
	blendFunc.AlphaFormat = 0;
	// 블랜딩 연산자 이것만 사용하면 된다 알고 있자
	blendFunc.BlendOp = AC_SRC_OVER;

	if (blendImage == NULL) {
		Release();
		return E_FAIL;
	}

	// 렌더에서 쓰고있는 이미지는 알파값 줄 수 없음
	// 알파값 줄 수 있는 이미지는 마젠타 뺄 수 없음
	// 그래서 마젠타 빼고 알파값 주는 식으로 할꺼

	tempDC = CreateCompatibleDC(hdc);

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

		DeleteDC(tempDC);
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

void Image::Render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		// transparent : 투명한
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/

		GdiTransparentBlt
		(
			// 화면 부분
			hdc,					// 복사될 장소의 DC
			destX,					// 복사될 좌표의 시작점 X
			destY,					// 복사될 좌표의 시작점 Y
			sourWidth,				// 복사될 이미지의 가로 크기
			sourHeight,				// 복사될 이미지의 세로 크기

									// 이미지 부분
			m_imageInfo->hMemDC,	// 복사될 대상의 DC
									// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			sourX,					// 복사될 이미지 시작점 X
			sourY,					// 복사될 이미지 시작점 Y
									// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			sourWidth,				// 복사될 이미지 가로크기
			sourHeight,				// 복사될 이미지 세로크기
			transColor				// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			m_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void Image::AlphaRender(HDC hdc, BYTE alpha)
{
	// 투명도를 얼마만큼 주겠느냐 설정
	blendFunc.SourceConstantAlpha = alpha;
	// 마젠타 지우고 할 꺼면
	if (isTrans) {
		// AlphaBlend() 특정색을 뺄 수 없이 투명도만 조절

		// 1. 출력 해야될 DC에 그려져 있는 내용을 블렌드 이미지에 그려준다
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, 0, 0, SRCCOPY); // 고속 복사 SRCCOPY

		// 2. 출력 해야될 이미지를 블렌드에 그려준다. (마젠타 값을 제거)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. 블랜드 DC를 출력해야될 DC에 그린다.
		AlphaBlend(
			hdc,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendImage->hMemDC, 
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	// 투명도를 얼마만큼 주겠느냐 설정
	blendFunc.SourceConstantAlpha = alpha;
	// 마젠타 지우고 할 꺼면
	if (isTrans) {
		// AlphaBlend() 특정색을 뺄 수 없이 투명도만 조절

		// 1. 출력 해야될 DC에 그려져 있는 내용을 블렌드 이미지에 그려준다
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, destX, destY, SRCCOPY); // 고속 복사 SRCCOPY

		// 2. 출력 해야될 이미지를 블렌드에 그려준다. (마젠타 값을 제거)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. 블랜드 DC를 출력해야될 DC에 그린다.
		AlphaBlend(
			hdc,
			destX,
			destY,
			m_imageInfo->width,
			m_imageInfo->height,
			blendImage->hMemDC, 
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	// 투명도를 얼마만큼 주겠느냐 설정
	blendFunc.SourceConstantAlpha = alpha;
	// 마젠타 지우고 할 꺼면
	if (isTrans) {
		// AlphaBlend() 특정색을 뺄 수 없이 투명도만 조절

		// 1. 출력 해야될 DC에 그려져 있는 내용을 블렌드 이미지에 그려준다
		BitBlt(blendImage->hMemDC, 0, 0, blendImage->width, blendImage->height,
			hdc, destX, destY, SRCCOPY); // 고속 복사 SRCCOPY

		// 2. 출력 해야될 이미지를 블렌드에 그려준다. (마젠타 값을 제거)
		GdiTransparentBlt(
			blendImage->hMemDC,
			0,
			0,
			sourWidth,
			sourHeight,
			m_imageInfo->hMemDC,
			sourX,
			sourY,
			sourWidth,
			sourHeight,
			transColor);
		// 3. 블랜드 DC를 출력해야될 DC에 그린다.
		AlphaBlend(
			hdc,
			destX,
			destY,
			sourWidth,
			sourHeight,
			blendImage->hMemDC, 
			0,
			0,
			sourWidth,
			sourHeight,
			blendFunc);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, sourWidth, sourHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight,
			blendFunc);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY)
{
	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		// transparent : 투명한
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/

		GdiTransparentBlt
		(
			// 화면 부분
			hdc,						// 복사될 장소의 DC
			destX,						// 복사될 좌표의 시작점 X
			destY,						// 복사될 좌표의 시작점 Y
			m_imageInfo->frameWidth,	// 복사될 이미지의 가로 크기
			m_imageInfo->frameHeight,	// 복사될 이미지의 세로 크기

										// 이미지 부분
			m_imageInfo->hMemDC,		// 복사될 대상의 DC
										// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			// 복사될 이미지 시작점 X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// 복사될 이미지 시작점 Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			m_imageInfo->frameWidth,	// 복사될 이미지 가로크기
			m_imageInfo->frameHeight,	// 복사될 이미지 세로크기
			transColor					// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		BitBlt(hdc, destX, destY,
			m_imageInfo->frameWidth, m_imageInfo->frameHeight,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight, SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;



	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		// transparent : 투명한
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/

		GdiTransparentBlt
		(
			// 화면 부분
			hdc,						// 복사될 장소의 DC
			destX,						// 복사될 좌표의 시작점 X
			destY,						// 복사될 좌표의 시작점 Y
			m_imageInfo->frameWidth,	// 복사될 이미지의 가로 크기
			m_imageInfo->frameHeight,	// 복사될 이미지의 세로 크기

										// 이미지 부분
			m_imageInfo->hMemDC,		// 복사될 대상의 DC
										// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			// 복사될 이미지 시작점 X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// 복사될 이미지 시작점 Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			m_imageInfo->frameWidth,	// 복사될 이미지 가로크기
			m_imageInfo->frameHeight,	// 복사될 이미지 세로크기
			transColor					// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		BitBlt(hdc, destX, destY,
			m_imageInfo->frameWidth, m_imageInfo->frameHeight,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight, SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	AlphaRender(hdc, destX, destY,
		currentFrameX * m_imageInfo->frameWidth,
		currentFrameY * m_imageInfo->frameHeight,
		m_imageInfo->frameWidth, m_imageInfo->frameHeight, alpha);
}

// drawArea 그려줄 부분
void Image::LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	// offset 값 음수가 나오면 안되서 보정 하기
	if (offsetX < 0)
		offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
	if (offsetY < 0)
		offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

	// 그려지는 영역(이미지)를 셋팅할 변수
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	// 그려지는 DC의 영역
	RECT rcDest;
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	// 세로 루프 영역 구하기
	for (int y = 0; y < drawAreaH; y += sourHeight) {
		// 영역의 높이 계산
		rcSour.top = (y + offsetY) % m_imageInfo->height;
		rcSour.bottom = m_imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		// sour영역이 그리는 화면을 넘어 갔다면
		if (y + sourHeight > drawAreaH) {
			// 넘어간 그림의 값만큼 bottom의 값을 올려준다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}
		// 그려지는 영역
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// 가로 루프 영역
		for (int x = 0; x < drawAreaW; x += sourWidth) {
			rcSour.left = (x + offsetX) % m_imageInfo->width;
			rcSour.right = m_imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			// sour영역이 그리는 화면을 넘어가면
			if (x + sourWidth > drawAreaW) {
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			// 그리기
			Render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void Image::LoopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	// offset 값 음수가 나오면 안되서 보정 하기
	if (offsetX < 0)
		offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
	if (offsetY < 0)
		offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

	// 그려지는 영역(이미지)를 셋팅할 변수
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	// 그려지는 DC의 영역
	RECT rcDest = {};
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	// 세로 루프 영역 구하기
	for (int y = 0; y < drawAreaH; y += sourHeight) {
		// 영역의 높이 계산
		rcSour.top = (y + offsetY) % m_imageInfo->height;
		rcSour.bottom = m_imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		// sour영역이 그리는 화면을 넘어 갔다면
		if (y + sourHeight > drawAreaH) {
			// 넘어간 그림의 값만큼 bottom의 값을 올려준다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}
		// 그려지는 영역
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// 가로 루프 영역
		for (int x = 0; x < drawAreaW; x += sourWidth) {
			rcSour.left = (x + offsetX) % m_imageInfo->width;
			rcSour.right = m_imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			// sour영역이 그리는 화면을 넘어가면
			if (x + sourWidth > drawAreaW) {
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			// 그리기
			AlphaRender(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top, alpha);
		}
	}
}

void Image::Render(HDC hdc, int destX, int destY, int width, int height)
{
	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {

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
		StretchBlt(hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			SRCCOPY);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, int width, int height, BYTE alpha)
{
	// 투명도를 얼마만큼 주겠느냐 설정
	blendFunc.SourceConstantAlpha = alpha;
	// 마젠타 지우고 할 꺼면
	if (isTrans) {
		// AlphaBlend() 특정색을 뺄 수 없이 투명도만 조절

		// blendImage에서 hMemDC에 hBit가 
		// m_imageInfo의 width height로 고정되서 그런거
		HBITMAP tempBitmap =
			(HBITMAP)CreateCompatibleBitmap(hdc, width, height);
		HBITMAP temOldBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);
	
		// 1. 출력 해야될 DC에 그려져 있는 내용을 블렌드 이미지에 그려준다
		BitBlt(tempDC, 0, 0, width, height,
			hdc, destX, destY, SRCCOPY); // 고속 복사 SRCCOPY

		// 2. 출력 해야될 이미지를 블렌드에 그려준다. (마젠타 값을 제거)
		GdiTransparentBlt(
			tempDC,
			0,
			0,
			width,
			height,
			m_imageInfo->hMemDC,
			0,
			0,
			m_imageInfo->width,
			m_imageInfo->height,
			transColor);
		// 3. 블랜드 DC를 출력해야될 DC에 그린다.
		AlphaBlend(
			hdc,
			destX,
			destY,
			width,
			height,
			tempDC,
			0,
			0,
			width,
			height,
			blendFunc);

		SelectObject(tempDC, temOldBitmap);
		DeleteObject(tempBitmap);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			blendFunc);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int width, int height)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	// 특정 색상을 빼고 그리겠다면
	if (isTrans) {
		//GdiTransparentBlt : 비트맵 출력시 특정 색상을 제외 하고 출력
		// transparent : 투명한
		/*
		bitmap은 alpha값이 없음 투명하게 만들 수가 없음
		내가 출력하는 캐릭터하고 배경하고 동시에 띄우려면 캐릭터 외부색상 지워야함
		*/

		GdiTransparentBlt
		(
			// 화면 부분
			hdc,						// 복사될 장소의 DC
			destX,						// 복사될 좌표의 시작점 X
			destY,						// 복사될 좌표의 시작점 Y
			width,						// 복사될 이미지의 가로 크기
			height,						// 복사될 이미지의 세로 크기

										// 이미지 부분
			m_imageInfo->hMemDC,		// 복사될 대상의 DC
										// 여기부분 수정하면 원하는 부분만 그릴 수 있음
			// 복사될 이미지 시작점 X
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			// 복사될 이미지 시작점 Y
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			// 이게 이미지 원본보다 크면 나머지 부분 흰색으로 채워짐
			m_imageInfo->frameWidth,	// 복사될 이미지 가로크기
			m_imageInfo->frameHeight,	// 복사될 이미지 세로크기
			transColor					// 복사할때 제외할 색상(기본적으로 마젠타 씀)
		);
	}
	// 원본 이미지 그대로 그냥 그리겠다면
	else {
		StretchBlt(hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth, 
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			SRCCOPY);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int width, int height, BYTE alpha)
{
	m_imageInfo->currentFrameX = currentFrameX;
	m_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > m_imageInfo->maxFrameX)
		m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
	if (currentFrameY > m_imageInfo->maxFrameY)
		m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;

	// 투명도를 얼마만큼 주겠느냐 설정
	blendFunc.SourceConstantAlpha = alpha;
	// 마젠타 지우고 할 꺼면
	if (isTrans) {
		// AlphaBlend() 특정색을 뺄 수 없이 투명도만 조절

		// blendImage에서 hMemDC에 hBit가 
		// m_imageInfo의 width height로 고정되서 그런거
		HBITMAP tempBitmap =
			(HBITMAP)CreateCompatibleBitmap(hdc, width, height);
		HBITMAP temOldBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

		// 1. 출력 해야될 DC에 그려져 있는 내용을 블렌드 이미지에 그려준다
		BitBlt(tempDC, 0, 0, width, height,
			hdc, destX, destY, SRCCOPY); // 고속 복사 SRCCOPY

		// 2. 출력 해야될 이미지를 블렌드에 그려준다. (마젠타 값을 제거)
		GdiTransparentBlt(
			tempDC,
			0,
			0,
			width,
			height,
			m_imageInfo->hMemDC,
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			transColor);
		// 3. 블랜드 DC를 출력해야될 DC에 그린다.
		AlphaBlend(
			hdc,
			destX,
			destY,
			width,
			height,
			tempDC,
			0,
			0,
			width,
			height,
			blendFunc);

		SelectObject(tempDC, temOldBitmap);
		DeleteObject(tempBitmap);
	}
	else {
		AlphaBlend(
			hdc, destX, destY, width, height,
			m_imageInfo->hMemDC, 
			m_imageInfo->currentFrameX * m_imageInfo->frameWidth, 
			m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
			m_imageInfo->frameWidth,
			m_imageInfo->frameHeight,
			blendFunc);
	}
}
