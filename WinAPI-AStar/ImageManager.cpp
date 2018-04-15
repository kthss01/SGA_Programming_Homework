#include "stdafx.h"
#include "ImageManager.h"


ImageManager::ImageManager()
{
}


ImageManager::~ImageManager()
{
}

void ImageManager::Release()
{
	DeleteAll();
}

Image * ImageManager::AddImage(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image;

	if (FAILED(img->Init(fileName, x, y, width, height, isTrans, transColor))) {
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList[strKey] = img;

	return img;
}

Image * ImageManager::AddImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	// 검색해서 이미 있으면 추가하지 않고 그냥 리턴
	Image* img = FindImage(strKey);

	if (img) return img;

	// SUCCEEDED() 인자값은 true false -> true이면 true false이면 false 반환
	// FAILED() 인자값 true or false -> true이면 false false이면 true 반환
	// HRESULT 일 경우 사용
	// S_OK E_FAILED, INVALID DATA(유효하지 않는 데이터) 이런것도 있음

	img = new Image;
	// 실패하면 true고 성공하면 false
	if (FAILED(
		img->Init(fileName, x, y, width, height,
			frameX, frameY, isTrans, transColor))) {
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList[strKey] = img;

	return img;
}

Image * ImageManager::FindImage(string strKey)
{
	// 코드 상으로는 이게 편하지만
	// iterator로 찾으면 메모리 적으로는 좋음 count는 다 확인해야되니
	if (_mImageList.count(strKey) < 1) return NULL;

	return _mImageList[strKey];
}

bool ImageManager::DeleteImage(string strKey)
{
	return false;
}

bool ImageManager::DeleteAll()
{
	map<string, Image*>::iterator iter = _mImageList.begin();

	for (; iter != _mImageList.end();) {
		if (iter->second != NULL) {
			iter->second->Release(); // IMAGE_INFO 내용 삭제
			SAFE_DELETE(iter->second); // image* 삭제
			iter = _mImageList.erase(iter); // map에서 삭제
		}
		else {
			iter++;
		}
	}
	_mImageList.clear(); // 목록을 한번 더 확실하게 지우기 위해서

	return true;
}

void ImageManager::Render(string strKey, HDC hdc, int destX, int destY)
{
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY);
}

void ImageManager::Render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::AlphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	Image* img = FindImage(strKey);
	if (img) img->AlphaRender(hdc, destX, destY, alpha);
}

void ImageManager::FrameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	Image* img = FindImage(strKey);
	if (img) img->FrameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}
