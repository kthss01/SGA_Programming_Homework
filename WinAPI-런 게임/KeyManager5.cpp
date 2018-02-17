#include "stdafx.h"
#include "KeyManager5.h"


KeyManager5::KeyManager5()
{
	for (int i = 0; i < KEYMAX; i++) {
		m_keyDown.set(i, false);
		m_keyUp.set(i, false);
	}
}


KeyManager5::~KeyManager5()
{
}

bool KeyManager5::GetKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) {
		if(!m_keyDown[key]) {
			m_keyDown.set(key, true);
			return true;
		}
	}
	else {
		m_keyDown.set(key, false);
	}
	return false;
}

bool KeyManager5::GetKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) {
		m_keyUp.set(key, true);
	}
	else {
		if (m_keyUp[key]) {
			m_keyUp.set(key, false);
			return true;
		}
	}
	return false;
}

bool KeyManager5::GetKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool KeyManager5::GetToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
