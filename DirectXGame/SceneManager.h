#pragma once
#include "Input.h"
#include "Audio.h"
#include "BaseScene.h"
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
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// シーン
	std::unique_ptr<BaseScene> m_pScene = nullptr;
	SCENE CurrentScene_;
	SCENE NextScene_;

};
