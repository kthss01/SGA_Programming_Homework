#include "stdafx.h"
#include "KeyManager3.h"


KeyManager3::KeyManager3()
{
	for (int i = 0; i < KEYMAX; i++) {
		m_keyDown.set(i, false);
		m_keyUp.set(i, false);
	}
}


KeyManager3::~KeyManager3()
{
}

bool KeyManager3::GetKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) {
		if (!m_keyDown[key]) {
			m_keyDown.set(key, true);
			return true;
		}
	}
	else {
		m_keyDown.set(key, false);
	}
	return false;
}

bool KeyManager3::GetKeyUp(int key)
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

bool KeyManager3::GetKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool KeyManager3::GetToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
