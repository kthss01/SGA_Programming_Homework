#include "stdafx.h"
#include "Input.h"

Input* Input::instance = NULL;

Input::Input( void )
{
	this->Init();
}

Input::~Input( void )
{
	this->Release();
}

Input * Input::Get()
{
	if (instance == NULL)
		instance = new Input;
	return instance;
}

void Input::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

HRESULT Input::Init( void )
{
	for ( int i = 0; i < MAX_KEY_NUM; i++ ) {
		m_bDownCheck[i] = false;
		m_bUpCheck[i] = false;
	}


	return S_OK;
}
void Input::Release( void )
{

}

bool Input::GetKey( int nVKey )
{
	return ( GetAsyncKeyState( nVKey ) & 0x8000 );
}

bool Input::GetKeyDown( int nVKey )
{
	if ( GetAsyncKeyState( nVKey ) & 0x8000 )
	{
		if ( m_bDownCheck[nVKey] == false )
		{
			m_bDownCheck[nVKey] = true;
			return true;
		}
	}

	else {
		m_bDownCheck[nVKey] = false;
	}

	return false;
}

bool Input::GetKeyUp( int nVKey )
{
	if ( GetAsyncKeyState( nVKey ) & 0x8000 )
	{
		if ( m_bUpCheck[nVKey] == false )
		{
			m_bUpCheck[nVKey] = true;
		}
	}
	else
	{
		if ( m_bUpCheck[nVKey] )
		{
			m_bUpCheck[nVKey] = false;
			return true;
		}
	}

	return false;
}
