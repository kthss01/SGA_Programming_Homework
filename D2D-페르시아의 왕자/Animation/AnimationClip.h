#pragma once
// 특정 애니메이션 행동을 종합적으로 포함해서 animation clip이라고 함

// AnimationClip에서 한 프레임
struct AnimationData {
	float frameRate;	// 내가 초당 몇개의 애니메이션을 보여줄 것인지
	Vector2 frame;
	Vector2 MaxFrame;

	wstring textureName;
	AnimationData* pNextFrame;
};

// 동작 하나 점프나 공격 이런게 모여 있는거
class AnimationClip
{
private:
	std::map<wstring, AnimationData*> aniClip;
public:
	AnimationClip();
	~AnimationClip();

	void PushAnimationData(wstring keyName, 
		AnimationData* data);
	AnimationData* GetCurrentData(wstring keyName);
};

