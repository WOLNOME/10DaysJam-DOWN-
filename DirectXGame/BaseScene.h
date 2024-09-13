#pragma once
#include "Input.h"
#include "Audio.h"
#include "Enum.h"

class BaseScene {
public:
	virtual ~BaseScene() = default;

	virtual void Initialize(Input* input,Audio* audio) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

public:
	virtual SCENE GetNextScene() = 0;
	virtual void SetNowPhase(int nowPhase) = 0;

protected:
	// それぞれのシーンにシーン遷移タイミングと遷移先を任せる
	SCENE NextScene;
	int nowPhase;
};
