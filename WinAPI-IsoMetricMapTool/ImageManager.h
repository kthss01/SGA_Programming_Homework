#pragma once

#include "SingletonBase.h"
#include "Image.h"

// 이미지 저장 및 호출
// 이미지 매니저는 이미지 보관 하고 프레임 렌더 정도만 하는 거라고 보면 됨

class ImageManager : public SingletonBase<ImageManager>
{
private:
	map<string, Image*> _mImageList;

public:
	ImageManager();
	~ImageManager();

	// init은 들어갈 내용없어서 설정 안함
	void Release();
	Image* AddImage(string strKey, const char* fileName,
		float x, float y, int width, int height,
		bool isTrans, COLORREF transColor);
	Image* AddImage(string strKey, const char* fileName,
		float x, float y, int width, int height,
		int frameX, int frameY, bool isTrans, COLORREF transColor);

	Image* FindImage(string strKey);
	bool DeleteImage(string strKey);
	bool DeleteAll();

	// 일반 렌더
	void Render(string strKey, HDC hdc, int destX, int destY);
	void Render(string strKey, HDC hdc, int destX, int destY,
		int sourX, int sourY, int sourWidth, int sourHeight);
	
	// 알파 렌더
	void AlphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha);

	// 프래임 렌더
	void FrameRender(string strKey, HDC hdc, int destX, int destY,
		int currentFrameX, int currentFrameY);
};

#define IMAGE ImageManager::GetSingleton()

