#pragma once
// 특정 애니메이션 행동을 종합적으로 포함해서 animation clip이라고 함

// 애니메이션이 동작하는 방식
enum AniRepeatType {
	AniRepeatType_END = 0, // 한번만 실행
	AniRepeatType_Loop,		// 계속해서 실행
	AniRepeatType_Reverse	// 처음부터 끝까지 간담에 역순으로 실행
};

// AnimationClip에서 한 프레임
struct AnimationData {
	wstring keyName;	// 내가 몇번째 프레임으로 짤랐는지 어택1, 어택2 ...
	Vector2 currentFrame;	// 한 data에 대한 프레임 값
	Vector2 maxFrame;	// 이미지에 대해서 몇장을 짜를건지
						// 내가 이 애니메이션에서 다음 동작으로 넘어가는데 걸리는 시간
						// frameRate에 영향을 받을꺼임
	float time = 0.2f;
};

// 동작 하나 점프나 공격 이런게 모여 있는거
class AnimationClip
{
private:
	wstring keyName;	// 공격, 점프, 무브 같은거
	float frameRate;	// 내가 초당 몇개의 애니메이션을 보여줄 것인지
	vector<AnimationData> aniClip;
	wstring textureName;

	int index;

	bool isPlay = false;

	float PlayTime;
	bool isReverse = false;
public:
	AnimationClip();
	~AnimationClip();

	void Play();

	bool IsPlay() { return isPlay; }

	void Update(AniRepeatType playType);

	void PushAnimationData(AnimationData data);
	AnimationData GetCurrentData();
};

