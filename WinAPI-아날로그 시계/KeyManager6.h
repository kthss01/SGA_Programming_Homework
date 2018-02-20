#pragma once

#include "SigletonBase.h"
#include <bitset>

using namespace std;

#define KEYMAX 256

class KeyManager6 : public SingletonBase<KeyManager6>
{
private:
	bitset<KEYMAX> m_keyUp;
	bitset<KEYMAX> m_keyDown;
public:
	KeyManager6();
	~KeyManager6();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKey(int key);
	bool GetToggleKey(int key);
};

#define INPUT KeyManager6::GetSingleton()
