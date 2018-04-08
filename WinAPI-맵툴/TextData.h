#pragma once

#include "SIngletonBase.h"

#define MAXSTRSIZE 32

class TextData : public SingletonBase<TextData>
{
private:
	HANDLE file;
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

	//void CreateWriteTextDataHandle(char* fileName) {
	//	file = CreateFile(fileName, GENERIC_WRITE,
	//		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//}
	//void CreateReadTextDataHandle(char* fileName) {
	//	file = CreateFile(fileName, GENERIC_READ,
	//		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//}
	//void TextWrite(vector<string> vStr);
	//vector< vector<string> > TextRead();
	//void CloseTextDataHandle() { CloseHandle(file); }

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

