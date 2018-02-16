#pragma once

#include "SingletonBase2.h"
#include <bitset>

using namespace std;

#define KEYMAX 256

class KeyManager2 : public SingletonBase2<KeyManager2>
{
private:
	bitset<KEYMAX> m_keyUp;
	bitset<KEYMAX> m_keyDown;
public:
	KeyManager2();
	~KeyManager2();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKey(int key);
	bool GetToggleKey(int key);
};

#define INPUT KeyManager2::GetSingleton()
