#pragma once

template<typename T>
class SingletonBase2 {
protected:
	static T* instance;
	SingletonBase2() {};
	~SingletonBase2() {};

public:
	static T* GetSingleton();
	void ReleaseSingleton();
};

template<typename T>
T* SingletonBase2<T>::instance = NULL;

template<typename T>
inline T * SingletonBase2<T>::GetSingleton()
{
	if (!instance) instance = new T;
	return instance;
}

template<typename T>
inline void SingletonBase2<T>::ReleaseSingleton()
{
	if (instance) {
		delete instance;
		instance = NULL;
	}
}
