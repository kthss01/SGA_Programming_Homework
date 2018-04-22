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
		DWORD resID;			// 리소스 ID
		HDC hMemDC;				// 메모리 DC
		HBITMAP hBit;			// 새로운 비트맵 핸들
		HBITMAP hOBit;			// 올드 비트맵 핸들

		float x;				// 이미지 x 좌표
		float y;				// 이미지 y 좌표
		int width;				// 이미지 가로크기
		int height;				// 이미지 세로크기
		int currentFrameX;		// 현재 프레임 X
		int currentFrameY;		// 현재 프레임 Y
		int maxFrameX;			// 최대 X 프레임 갯수
		int maxFrameY;			// 최대 Y 프레임 갯수
		int frameWidth;			// 1 프레임 당 가로 크기	* 화면에 뜨는 크기
		int frameHeight;		// 1 프레임 당 세로 크기

		BYTE loadType;			// 로드 타입

		tagImage() {
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0.0f;
			y = 0.0f;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
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

	HDC tempDC;
	HBITMAP tempBitmap;
	HBITMAP tempOBitmap;
public:
	Image();
	~Image();

	// 빈 비트맵 이미지 초기화
	HRESULT Init(int width, int height);
	// 파일 이름으로 비트맵 이미지 초기화
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));

	HRESULT Init(const char* fileName, float x, float y, int width, int height,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));

	// 프레임 이미지 파일로 초기화
	HRESULT Init(const char* fileName,
		int width, int height, int frameX, int frameY,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(const char* fileName, float x, float y,
		int width, int height, int frameX, int frameY,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));

	void SetTransColor(bool isTrans, COLORREF transColor) {
		this->isTrans = isTrans;
		this->transColor = transColor;
	}

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

	void AlphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void AlphaRender(HDC hdc, int destX, int destY, int sourX, int sourY,
		int sourWidth, int sourHeight, BYTE alpha);

	// 프레임 랜더
	void FrameRender(HDC hdc, int destX, int destY);

	void FrameRender(HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY);

	void FrameRender(HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY, BYTE alpha);

	// 루프 렌더
	// LPRECT == RECT*
	void LoopRender(HDC hdc, const LPRECT drawArea,
		int offsetX, int offsetY);
	void LoopAlphaRender(HDC hdc, const LPRECT drawArea,
		int offsetX, int offsetY, BYTE alpha);

	// 크기 변환
	void Render(HDC hdc, int destX, int destY, int width, int height);
	void AlphaRender(HDC hdc, int destX, int destY, int width, int height, BYTE alpha);
	void FrameRender(HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY, int width, int height);
	void FrameRender(HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY, int width, int height, BYTE alpha);

	inline HDC GetMemDC() { return m_imageInfo->hMemDC; }

	// Get, Set 함수들 (나중에)

	// 이미지의 좌표 x get set
	inline float GetX() { return m_imageInfo->x; }
	inline void SetX(float x) { m_imageInfo->x = x; }
	// 이미지의 좌표 y get set
	inline float GetY() { return m_imageInfo->y; }
	inline void SetY(float y) { m_imageInfo->y = y; }
	// 이미지 센터 좌표 셋팅 set
	inline void SetCenter(float x, float y) {
		m_imageInfo->x = x - m_imageInfo->width / 2;
		m_imageInfo->y = y - m_imageInfo->height / 2;
	}

	// 이미지 가로, 세로 얻기 Get
	inline int GetWidth() { return m_imageInfo->width; }
	inline int GetHeight() { return m_imageInfo->height; }

	// 바운딩 박스 (충돌용 렉트 만들기) Get
	// 렉트 크기로 되어있는 박스
	// x, y는 중심점으로 되어 있음
	inline RECT GetBoundingBox() {
		RECT rc = RectMakeCenter(
			m_imageInfo->x, m_imageInfo->y,
			m_imageInfo->width, m_imageInfo->height);
		return rc;
	}

	// 프레임 바운딩 박스 (프레임 이미지용 충돌 렉트) Get
	// 좌표값은 프레임 x,y와 너비랑 높이 
	inline RECT GetFrameBoundingBox() {
		RECT rc = RectMakeCenter(
			m_imageInfo->x, m_imageInfo->y,
			m_imageInfo->frameWidth, m_imageInfo->frameHeight);
		return rc;
	}

	// 프레임 X get set
	inline int GetFrameX() { return m_imageInfo->currentFrameX; }
	inline void SetFrameX(int frameX) { m_imageInfo->currentFrameX = frameX; }
	// 프레임 Y get set
	inline int GetFrameY() { return m_imageInfo->currentFrameY; }
	inline void SetFrameY(int frameY) { m_imageInfo->currentFrameY = frameY; }
	// 프레임 가로 세로 크기 Get
	inline int GetFrameWidth() { return m_imageInfo->frameWidth; }
	inline int GetFrameHeight() { return m_imageInfo->frameHeight; }
	// 맥스 프레임 x, y, Get
	inline int GetMaxFrameX() { return m_imageInfo->maxFrameX; }
	inline int GetMaxFrameY() { return m_imageInfo->maxFrameY; }
};