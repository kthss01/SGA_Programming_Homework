#include "Potion.h"



Potion::Potion()
{
}


Potion::~Potion()
{
}

void Potion::Show()
{
	cout << "이름	: " << name << endl;
	cout << "가격	: " << price << endl;
	cout << "HP회복	: " << stat << endl;
	cout << "설명	: " << comment << endl;
	cout << "갯수	: ";
	if (count == 0)
		cout << "매진";
	else
		cout << count;
	cout << endl;
}

void Potion::InitType()
{
	type = HP;
}
