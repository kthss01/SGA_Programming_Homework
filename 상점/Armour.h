#pragma once
#include "Item.h"
class Armour : public Item
{
public:
	Armour();
	~Armour();

	void Show() override;

	// Item을(를) 통해 상속됨
	virtual void InitType() override;
};

