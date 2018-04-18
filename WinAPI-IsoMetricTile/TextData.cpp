#include "stdafx.h"
#include "TextData.h"


TextData::TextData()
{
}


TextData::~TextData()
{
}

HRESULT TextData::Init()
{
	return S_OK;
}

void TextData::TextSave(char * saveFileName, vector<string> vStr)
{
	// api가 가지고있는 save 함수 사용
	HANDLE file;
	char str[MAXSTRSIZE];
	DWORD write;

	// 복사해주는데 크기까지 설정가능한 거
	strncpy_s(str, MAXSTRSIZE, VectorArrayCombine(vStr), MAXSTRSIZE);

	// CreateFile Binary 형식으로 들어감
	// Binary 16진수 데이터 형식으로 들어감
	// Binary 한글 관련으로 문제가 있었음
	file = CreateFile(
		saveFileName,	// 생성할 파일 또는 로드할 파일의 이름
		GENERIC_WRITE,	// 파일이나 장치를 만들거나 열때의 권한
		0,				// 파일 공유 모드 입력
		NULL,			// 파일 또는 장치를 열 때 보안 및 특성
						// 항상 처음부터 작성하겠다는거 다 지우고
						// 여기 바꿔주면 라인별로 넣을 수 있음
		CREATE_ALWAYS,	// 파일이나 장치를 열 때 취할 행동
		FILE_ATTRIBUTE_NORMAL,	// 파일, 장치를 열 때, 만들 때 갖는 특성
		NULL			// 만들어질 파일이 갖게 될 특성 etc 저장되는 핸들
	);

	WriteFile(file, str, MAXSTRSIZE, &write, NULL);
	CloseHandle(file);
}

char * TextData::VectorArrayCombine(vector<string> vArray)
{
	char str[MAXSTRSIZE];
	ZeroMemory(str, sizeof(str));

	for (int i = 0; i < vArray.size(); i++) {
		strncat_s(str, MAXSTRSIZE, vArray[i].c_str(), MAXSTRSIZE);

		// 마지막 문자는 제외하려고 해서
		if ((i + 1) < vArray.size()) {
			strcat_s(str, ",");
		}
	}

	return str;
}

vector<string> TextData::TextLoad(char * loadFileName)
{
	HANDLE file;
	char str[MAXSTRSIZE];
	DWORD read;

	ZeroMemory(str, sizeof(str));

	file = CreateFile(
		loadFileName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	ReadFile(file, str, MAXSTRSIZE, &read, NULL);
	CloseHandle(file);

	return CharArraySeparation(str);
}

vector<string> TextData::CharArraySeparation(char charArray[])
{
	vector<string> vArray;
	char* separator = (char*)",";

	char* context = NULL;

	char* token = strtok_s(charArray, separator, &context);
	vArray.push_back(token);
	while (token) {
		token = strtok_s(NULL, separator, &context);
		if (token != NULL)
			vArray.push_back(token);
	}

	return vArray;;
}

void TextData::TextWrite(vector<string> vStr)
{
	char str[MAXSTRSIZE];

	ZeroMemory(str, sizeof(str));

	strcpy_s(str, MAXSTRSIZE, VectorArrayCombine(vStr));
	fprintf(fp, "%s\n", str);
}

vector<vector<string>> TextData::TextRead()
{
	vector< vector<string> > vvStr;

	char str[MAXSTRSIZE];

	while (fscanf_s(fp, "%s", &str, MAXSTRSIZE) != EOF) {
		vvStr.push_back(CharArraySeparation(str));
	}

	return vvStr;
}

