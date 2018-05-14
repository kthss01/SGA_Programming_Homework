#pragma once

#define MAX_KEY_NUM		256

class Input
{
private:
	bool			m_bDownCheck[MAX_KEY_NUM];
	bool			m_bUpCheck[MAX_KEY_NUM];

private:
	Input( void );
	~Input( void );

	static Input* instance;
public:
	static Input * Get();
	static void Delete();

	HRESULT Init( void );
	void Release( void );

	bool GetKey( int nVKey );
	bool GetKeyDown( int nVKey );
	bool GetKeyUp( int nVKey );
};

#define INPUT Input::Get()