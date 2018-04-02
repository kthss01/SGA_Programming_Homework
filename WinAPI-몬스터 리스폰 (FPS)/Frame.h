#pragma once

// fps
/*
장르를 뜻할때는 1인칭 슈팅 게임 (총싸움 같은거)

frame pre second 1초당 몇 프레임이 지나갔는지
게임에 대한 성능
최소 조건 30 fps
콘솔 게임 경우 그래픽 성능 높이고 최소 프레임 맞추게 됨
평균 프레임 60 fps 일반적으로 인식하는 최대 프레임이 60fps

30 fps
1초 동안 사진 연속으로 30개 보는거
60 이상 120 240은 보는건 차이 없는데 컴퓨터 성능만 많이 잡아먹는거

animation -> frame 가지고 있음
한 동작을 1초 동안 몇장으로 보여줄 껀지
*/

class Frame
{
private:
	// 일반 타이머 1/1000 초 고성능 타이머 1/백만
	// 컴퓨터나 노트북 cpu에 내장되어있음 옛날 꺼는 지원 안해주는거 있음
	// 고성능 타이머 쓴다고 무조건 좋은건 아님 cpu 부담이 늘기 때문에

	// 계산을 위한 녀석
	bool _isHardWare;			// 고성능 타이머를 지원 하는지?
								// 한 프레임 당 초단위로 계산 
	float _timeScale;			// 시간 경과량 
								// gettickCount() 1/1000 초 고성능 1/백만 초
	float _timeElapsed;			// 한 프레임당 경과량
								// __int64 기본 2^32 인데 2^64로 바꾼거 long과 같음
	__int64 _curTime;			// 현재 시간
	__int64 _lastTime;			// 마지막 시간
								// getTickcount면 1000 고성능이면 백만
	__int64 _periodFrequency;	// 시간 주기

								// timeElapsed = curTime - lastTime;

								// 가져다 쓸 녀석
	unsigned long _frameRate; // fps
							  // 이 값을 fps로 넘겨줄꺼
	unsigned long _FPSFrameCount; // 성능이 떨어져 지정된 값보다 작아지면 count++; 
	float _FPSTimeElapsed;
	float _worldTime;
public:
	Frame();
	~Frame();

	HRESULT Init();

	// fps 제한을 걸어줄 녀석
	// 얘가 균등하게 나눠주진 않음
	// 제한 풀고 싶으면 0으로 아닌 경우 제한하고 싶은 프레임 인자로
	void Tick(float lockFPS = 0.0f);

	// 함수명 뒤에 const 붙인거
	// 값을 변경하지 못함, 
	// 초기화해서 (new 해서) 바로 넣어주는 경우 있음 
	// 가끔 안되는 경우가 있는데 const로 하면 잘됨
	// FPS
	unsigned long GetFrameRate(char* str = NULL) const;
	// 한 프레임당 경과 시간
	// 2D에서는 상관없는데 3D에서는 곱해주는게 좋음
	float GetElapsedTime() const { return _timeElapsed; }
	// 전체 경과 시간 (창을 키고 난 후부터 계속해서 증가되는 시간)
	float GetWorldTime() const { return _worldTime; }
};

