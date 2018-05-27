#include "stdafx.h"
#include "String.h"


String::String()
{
}


String::~String()
{
}

wstring String::StringToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());
	return temp;
}

string String::WStringToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());
	return temp;
}
