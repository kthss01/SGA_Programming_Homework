#pragma once

#include "SIngletonBase.h"
// 헤더랑 cpp 합친거
#include "./FMOD/inc/fmod.hpp"
#pragma comment(lib, "./FMOD/lib/fmodex_vc.lib")

// fmod
/*
미리 만들어져있는거
사운드 플레이 하기 위해 도와주는 녀석
DirectX랑 같음
옛날에 만들어져서 개발이 중단되었으나 요즘에도 많이 사용하고 있음
간단하게 사용할 수 있고 지원하는 확장자가 많아서 자주 쓰고 있는거

사운드 편집 가능
fmod studio 사이트 가보면 확인 가능
*/

// 외부에서 include 해야 되는것들은 보통 네임스페이스도 같이 있음
using namespace FMOD;

// 미리 여유분으로 잡아두는 사운드가 총 20개라고 보면됨 + 5
// 많이 잡아두는거는 다양한 효과음이 들어가서
// 또는 사운드가 씹혀서 안나오는 경우 때문에 크게 크게 잡는거
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 20

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class SoundManager : public SingletonBase<SoundManager>
{
private:
	map<string, Sound**> m_totalSounds;

	// fmod 사용할지 안할지 fopen 에서 file 같은거
	// fmod 사용하기위한 시작지점, 끝지점
	System* m_system;
	// 더블 포인터를 쓰는 이유는
	// 사운드로 들어가는 인자값이 더블 포인터로 되어있어서
	Sound** m_sound;		// 사운드 1 개 (앨범에 노래 하나)
	Channel** m_channel;	// 앨범 자체를 나타내는거
public:
	SoundManager();
	~SoundManager();

	HRESULT Init();
	void Release();
	void Update();	// play가 되는 녀석

					// bgm은 bgm으로 쓸건지 효과음으로 쓸건지
					// loop는 반복할건지
	void AddSound(string keyName, string fileName,
		bool bgm = FALSE, bool loop = FALSE);
	// volume 소리크기 지정
	// volume = 0.0 ~ 1.0f 1.0f으로 했는데도 파일자체 소리를 키워야함
	void Play(string keyName, float volume = 1.0f);
	void Stop(string keyName);		// 정지 그 사운드값의 처음으로 돌아감
	void Pause(string keyName);		// 일시정지 그 위치의 멈추는거
	void Resume(string keyName);	// 다시 재생

	bool IsPlaySound(string keyName);
	bool isPauseSound(string keyName);
};

#define SOUND SoundManager::GetSingleton()