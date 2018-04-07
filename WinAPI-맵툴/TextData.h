#pragma once

#include "SIngletonBase.h"

class TextData : public SingletonBase<TextData>
{
private:

public:
	TextData();
	~TextData();

	HRESULT Init();

	// Save
	void TextSave(char* saveFileName, vector<string> vStr);
	char* VectorArrayCombine(vector<string> vArray);

	// Load
	// 한 줄에 넣을 데이터
	vector<string> TextLoad(char* loadFileName);
	// 데이터들의 리스트
	vector<string> CharArraySeparation(char charArray[]);
};

#define TEXTDATA TextData::GetSingleton()

