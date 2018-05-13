#pragma once
class String
{
public:
	String();
	~String();

	// 같은 방법을 하면 반대로도 바꿀 수 있음
	static wstring StringToWString(string value);
};

