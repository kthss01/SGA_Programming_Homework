#pragma once

// Image.h

// 이미지 띄우기

class Image {
	enum IMAGE_LOAD_KIND {
		LOAD_RESOURCE = 0,
		LOAD_FILE,
		LOAD_EMPTY,
		LOAD_END
	};

	typedef struct tagImage {
		DWORD resID;
		HDC hMemDC;
		HBITMAP hBit;
		HBITMAP hOBit;
		int width;
		int height;
		BYTE loadType;

		tagImage() {
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			width = 0;
			height = 0;
			loadType = LOAD_RESOURCE;
		}
	} IMAGE_INFO, *LPIMAGE_INFO;
private:
	LPIMAGE_INFO m_imageInfo;
	char* fileName;				// 로드할 파일(이미지) 이름
	bool isTrans;				// 배경색(마젠타 #define 때 색)를 지울 꺼냐?
	COLORREF transColor;		// 배경색 없앨 RGB(255,0,255) = 마젠타
								// 딱 마젠타 값만큼만 지워짐 비슷한 색은 안지워짐
								// 마젠타 범위로 할수도 있긴함
								// 다른 dc에 저장하고 제외하는 식으로 하면 할 수 있음

	BLENDFUNCTION blendFunc;	// 알파 블렌드를 위한 정보
	LPIMAGE_INFO blendImage;	// 알파 블렌드를 사용하기 위한 이미지 정보
public:
	Image();
	~Image();

	HRESULT Init(int width, int height);
	// 파일 이름으로 비트맵 이미지 초기화
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));

	void Release();

	// 일반 렌더
	void Render(HDC hdc);
	// 화면에서의 x ,y 값 만큼 이미지 렌더 시켜주는거
	void Render(HDC hdc, int destX, int destY);

	void Render(HDC hdc, int destX, int destY, int sourX, int sourY,
		int sourWidth, int sourHeight);

	// 알파 렌더
	// alpha가 alpha값 이라고 생각 byte이므로 0 ~ 255 
	void AlphaRender(HDC hdc, BYTE alpha);

	inline HDC GetMemDC() { return m_imageInfo->hMemDC; }
};