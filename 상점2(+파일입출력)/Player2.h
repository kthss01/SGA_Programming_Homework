#pragma once

#include "Unit.h"
#include "Inventory.h"

class Player2 : public Unit, public Inventory
{
public:
	Player2();
	~Player2();

	void InputPlayer();
	void ShowStatus();
};

