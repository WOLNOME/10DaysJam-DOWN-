#pragma once
#include "Input.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "BaseScene.h"
#include "Gradation.h"
#include <memory>

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Update();
	void Draw();

public:
	void ChangeScene();

private:
	//
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// シーン
	std::unique_ptr<BaseScene> m_pScene = nullptr;
	SCENE CurrentScene_;
	SCENE NextScene_;

	// シーン遷移
	std::unique_ptr<Gradation> gradation_ = nullptr;
	const int kAnimationFrame_ = 80;
	int AnimationFrame_;
	bool isInNow_;
	bool isOutNow_;

private:
	//現在のフェーズ
	int nowPhase

};
