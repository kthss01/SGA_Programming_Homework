#pragma once

#include "SingletonBase.h"
#include <bitset>

using namespace std;

#define KEYMAX 256

class KeyManager3 : public SingletonBase<KeyManager3> 
{
private:
	bitset<KEYMAX> m_keyUp;
	bitset<KEYMAX> m_keyDown;
public:
	KeyManager3();
	~KeyManager3();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKey(int key);
	bool GetToggleKey(int key);
};

#define INPUT KeyManager3::GetSingleton()

