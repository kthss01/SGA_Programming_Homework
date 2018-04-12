#pragma once

#include "SIngletonBase.h"

#define MAXSTRSIZE 128

class TextData : public SingletonBase<TextData>
{
private:
	FILE * fp;
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

	// 기존 파일 입출력 방식
	void CreateWriteTextDataFilePointer(char * fileName) {
		fopen_s(&fp, fileName, "w");
	}
	void TextWrite(vector<string> vStr);
	vector< vector<string> > TextRead();
	void CreateReadTextDataFilePointer(char * fileName) {
		fopen_s(&fp, fileName, "r");
	}
	void CloseTextDataFilePointer() { fclose(fp); }
};

#define TEXTDATA TextData::GetSingleton()

