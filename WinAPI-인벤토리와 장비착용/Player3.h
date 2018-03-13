#pragma once

#include "Unit.h"
#include "Inventory3.h"

class Player3 : public Unit, public Inventory3
{
public:
	Player3();
	~Player3();

	void InputPlayer();
};

