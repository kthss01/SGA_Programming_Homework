#pragma once

template<typename T>
class SingletonBase {
protected:
	static T* instance;
	SingletonBase() {};
	~SingletonBase() {};

public:
	static T* GetSingleton();
	void ReleaseSingleton();
};

template<typename T>
T* SingletonBase<T>::instance = NULL;

template<typename T>
T* SingletonBase<T>::GetSingleton() {
	if (!instance) instance = new T;
	return instance;
}

template<typename T>
void SingletonBase<T>::ReleaseSingleton() {
	if (instance) {
		delete instance;
		instance = 0;
	}
}