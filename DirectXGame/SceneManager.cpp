#include "SceneManager.h"
#include "GameScene.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
	//初期化
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//シーンの生成
	m_pScene = std::make_unique<GameScene>();
	//シーンの初期化
	m_pScene->Initialize(input_, audio_);
	CurrentScene_ = Game;
	NextScene_ = Game;
}

void SceneManager::Update() {
	// シーンの更新処理
	m_pScene->Update();

	// 各シーンからNextSceneの情報を受け取る
	if (NextScene_ != m_pScene->GetNextScene()) {
		NextScene_ = m_pScene->GetNextScene();
	}
	// シーンの切り替え処理
	ChangeScene();
}

void SceneManager::Draw() {
	//各シーンの描画
	m_pScene->Draw();
}

void SceneManager::ChangeScene() {}
