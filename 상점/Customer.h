#pragma once
#include <iostream>
#include <vector>

#include "Item.h"

using namespace std;

class Customer
{
private:
	vector<Item> inventory;
	int gold;
public:
	Customer();
	~Customer();

	void ShowInventory();

	int GetGold();

	void SetGold(int gold);

	void InsertItem(Item item);
	void DeleteItem(int index);

	Item GetItem(int index);
	int GetItemCount();
};

