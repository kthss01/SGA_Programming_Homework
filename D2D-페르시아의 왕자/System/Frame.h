#pragma once

class Frame
{
private:
	bool			m_bHardWare;		//고성능 타이머 지원 여부

	__int64			m_nPreriodTime;		//1초에 몇번카운트 하는가?
	__int64			m_nLastTime;		//마지막 프레임 시간
	__int64			m_nCurTime;			//현제 시간
	__int64			m_nDeltaTime;		//이전 프레임과 지난시간

	double			m_TimeScaleSec;		//1카운트 당초.
	double			m_FrameDeltaSec;	//한프레임 경과 초
	double			m_TotalSec;			//프로그램 시작후 경과시간.

	double			m_FrameCountSec;	//프레임 갱신 시간
	DWORD			m_FrameCount;		//프레임 카운트
	DWORD			m_FramePerSec;		//초당 프레임

private:
	Frame( void );
	~Frame( void );

	static Frame* instance;

public:

	static Frame* Get();
	static void Delete();

	HRESULT Init( void );
	void Release( void );

	//매업데이트 물려준다.
	void UpdateTime( float frameLock = 0.0f );

	//타임정보를 출력한다
	void DrawTimeInfo( HDC hdc );

	//프레임간의 지난 초시간을 얻는다.
	double GetFrameDeltaSec( void ) { return m_FrameDeltaSec; }


	void DrawShadowText( HDC hdc, int x, int y, char* szText, COLORREF textColor );

	double GetNowRealTimeSec();

};