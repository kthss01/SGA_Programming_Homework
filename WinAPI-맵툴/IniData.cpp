#include "stdafx.h"
#include "IniData.h"


IniData::IniData()
{
}


IniData::~IniData()
{
}

void IniData::AddData(const char * section, const char * key, const char * value)
{
	tagIniData iniData;
	//iniData.section = section;
	//iniData.key = key;
	//iniData.value = value;

	strcpy_s(iniData.section, sizeof(iniData.section), section);
	strcpy_s(iniData.key, sizeof(iniData.key), key);
	strcpy_s(iniData.value, sizeof(iniData.value), value);

	this->_vIniData.push_back(iniData);
}

void IniData::SaveData(const char * fileName)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C에서 내 프로젝트까지의 경로 가져오는 함수
	// 파일이 D에있음 d부터 
	GetCurrentDirectory(256, str);
	// save 폴더는 만들어줘야함
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	for (int i = 0; i < _vIniData.size(); i++) {
		WritePrivateProfileString(
			_vIniData[i].section,
			_vIniData[i].key,
			_vIniData[i].value,
			str);
	}
	_vIniData.clear();
}

char * IniData::LoadDataString(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C에서 내 프로젝트까지의 경로 가져오는 함수
	// 파일이 D에있음 d부터 
	GetCurrentDirectory(256, str);
	// save 폴더는 만들어줘야함
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	char data[64] = { 0 };

	GetPrivateProfileString(
		section,
		key,
		"",			// value 값 없을때 어떤 문자로 처리할지
		data,		// 어디다 저장 할지
		64,			// 크기
		str			// 경로
	);

	return data;
}

int IniData::LoadDataInteger(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];

	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));

	// C에서 내 프로젝트까지의 경로 가져오는 함수
	// 파일이 D에있음 d부터 
	GetCurrentDirectory(256, str);
	// save 폴더는 만들어줘야함
	sprintf_s(dir, "/save/%s.ini", fileName);
	strcat_s(str, dir);

	// 실수형은 스트링으로 읽어와서 변경을 해야함
	return GetPrivateProfileInt(section, key, 0, str);
}
