#include "Armour.h"



Armour::Armour()
{
}


Armour::~Armour()
{
}

void Armour::Show()
{
	cout << "이름	: " << name << endl;
	cout << "가격	: " << price << endl;
	cout << "방어력	: " << stat << endl;
	cout << "설명	: " << comment << endl;
	cout << "갯수	: ";
	if (count == 0)
		cout << "매진";
	else
		cout << count;
	cout << endl;
}

void Armour::InitType()
{
	type = DEF;
}
