#pragma once
#include "Item.h"
class Weapon : public Item
{
public:
	Weapon();
	~Weapon();

	void Show() override;

	// Item을(를) 통해 상속됨
	virtual void InitType() override;
};

