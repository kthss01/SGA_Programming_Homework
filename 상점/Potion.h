#pragma once
#include "Item.h"
class Potion : public Item
{
public:
	Potion();
	~Potion();

	void Show() override;

	// Item을(를) 통해 상속됨
	virtual void InitType() override;
};

