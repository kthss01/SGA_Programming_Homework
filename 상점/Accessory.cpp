#include "Accessory.h"



Accessory::Accessory()
{
}


Accessory::~Accessory()
{
}

void Accessory::Show()
{
	cout << "이름	: " << name << endl;
	cout << "가격	: " << price << endl;
	cout << "마력	: " << stat << endl;
	cout << "설명	: " << comment << endl;
	cout << "갯수	: ";
	if (count == 0)
		cout << "매진";
	else
		cout << count;
	cout << endl;
}

void Accessory::InitType()
{
	type = MP;
}
