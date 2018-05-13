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
