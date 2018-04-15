#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	:m_system(NULL), m_channel(NULL), m_sound(NULL)
{
}


SoundManager::~SoundManager()
{
}

HRESULT SoundManager::Init()
{
	// 사운드 시스템 생성
	System_Create(&m_system);

	// flag는 옵션
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	m_sound = new Sound*[TOTALSOUNDBUFFER];
	m_channel = new Channel*[TOTALSOUNDBUFFER];

	// ZeroMemory로 해도됨
	/// memset ZeroMemory로 초기화시 문제 발생함
	/// 그냥 반복문 돌려서 초기화 시킴
	//memset(m_sound, 0, sizeof(Sound*) * TOTALSOUNDBUFFER);
	//memset(m_channel, 0, sizeof(Channel*) * TOTALSOUNDBUFFER);
	//ZeroMemory(m_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	//ZeroMemory(m_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);
	for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
		m_sound[i] = NULL;
		m_channel[i] = NULL;
	}

	return S_OK;
}

void SoundManager::Release()
{
	if (m_channel != NULL || m_sound != NULL) {
		for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
			if (m_channel != NULL) {
				// fmod는 사운드 관리해야할 부분을 
				// channel이 가지고 있는 경우가 있음
				if (m_channel[i]) m_channel[i]->stop();
			}
			if (m_sound != NULL) {
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}
	SAFE_DELETE_ARRAY(m_sound);
	SAFE_DELETE_ARRAY(m_channel);

	if (m_system != NULL) {
		m_system->release();
		m_system->close();
	}

	// test
	// fmod 에서 3d 쓰고 싶으면 이런거 설정하면 된다고 함
	// 3d 음향 거리값 설정 가능
	//m_channel[0]->set3DMinMaxDistance()
	// 3d 음향 위치값 설정 가능
	//m_channel[0]->set3DCustomRolloff()
	// 도플러 효과도 줄 수 있음
}

void SoundManager::Update()
{
	m_system->update();
}

void SoundManager::AddSound(string keyName, string fileName, bool bgm, bool loop)
{
	// stream sound 차이는 크게 없는데
	// stream은 sound가 일렬로 쭉 저장되는데
	// sound는 그냥 저장된다고 보면됨
	// 저장되는 방식이 다름

	if (loop) {
		if (bgm) {
			// string.c_str() string char* 형으로 변경 해주는거
			m_system->createStream(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
		else {
			m_system->createSound(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
	}
	else {
		m_system->createSound(fileName.c_str(),
			FMOD_DEFAULT, NULL,
			&m_sound[m_totalSounds.size()]);
	}
	m_totalSounds[keyName] = &m_sound[m_totalSounds.size()];
}

void SoundManager::Play(string keyName, float volume)
{
	// channel에서 플레이해야되서 count도 돌리는거
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			// false는 바로 일시정지 시킬지 결정
			m_system->playSound(FMOD_CHANNEL_FREE,
				*iter->second, false, &m_channel[count]);
			m_channel[count]->setVolume(volume);
			break;
		}
	}
}

void SoundManager::Stop(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string keyName)
{
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaySound(string keyName)
{
	bool isPlay;

	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::isPauseSound(string keyName)
{
	bool isPaused;

	int count = 0;
	map<string, Sound**>::iterator iter;
	for (iter = m_totalSounds.begin();
		iter != m_totalSounds.end(); ++iter, count++) {
		if (keyName == iter->first) {
			m_channel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}
