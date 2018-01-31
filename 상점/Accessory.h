#pragma once
#include "Item.h"
class Accessory : public Item
{
public:
	Accessory();
	~Accessory();

	void Show() override;

	// Item을(를) 통해 상속됨
	virtual void InitType() override;
};

