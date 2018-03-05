#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager() {
	for (int i = 0; i < KEYMAX; i++) {
		m_keyDown.set(i, false);
		m_keyUp.set(i, false);
	}
}

KeyManager::~KeyManager() {

}

bool KeyManager::GetKeyDown(int key) {
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

bool KeyManager::GetKeyUp(int key) {
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

bool KeyManager::GetKey(int key) {
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool KeyManager::GetToggleKey(int key) {
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}