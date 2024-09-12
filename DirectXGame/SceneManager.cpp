#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "RestScene.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
	//初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//シーンの生成
	m_pScene = std::make_unique<TitleScene>();
	//シーンの初期化
	m_pScene->Initialize(input_, audio_);
	CurrentScene_ = Title;
	NextScene_ = Title;
	// シーン遷移生成
	gradation_ = std::make_unique<Gradation>();
	AnimationFrame_ = 0;
	isInNow_ = false;
	isOutNow_ = false;
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
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//各シーンの描画
	m_pScene->Draw();
	// シーン遷移描画
	gradation_->Draw(commandList);
}

void SceneManager::ChangeScene() {
	// シーン切り替えアニメーションとか作るならこの関数に書く

	// もしNextSceneが入力されているならシーン遷移アニメーションに入る
	if (!isInNow_ && NextScene_ != CurrentScene_) {
		AnimationFrame_ = 0;
		isInNow_ = true;
		isOutNow_ = false;
		gradation_->Initialize(KindAni::ADMISSION);
		gradation_->SetIsDraw(true);
	}

	// インアニメーション
	if (isInNow_) {
		AnimationFrame_++;

		// アニメーション処理
		gradation_->Update(AnimationFrame_, kAnimationFrame_);
		// アニメーション終わった時の処理
		if (AnimationFrame_ == kAnimationFrame_) {
			isInNow_ = false;
			isOutNow_ = true;
			AnimationFrame_ = 0;
		}
	}

	// 切り替え
	if (!isInNow_ && isOutNow_ && NextScene_ != CurrentScene_) {
		// 現在のシーンの削除
		if (m_pScene != NULL) {
			m_pScene.reset();
		}
		// 次のシーンの挿入
		switch (NextScene_) { // 引数のシーン
		case SCENE::Title:
			m_pScene = std::make_unique<TitleScene>(); // タイトルシーンを現在のシーンにする
			m_pScene->Initialize(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::Game:
			m_pScene = std::make_unique<GameScene>(); // ステージシーンを現在のシーンにする
			m_pScene->Initialize(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();
			;
			break;
		case SCENE::Rest:
			m_pScene = std::make_unique<RestScene>(); // レストシーンを現在のシーンにする
			m_pScene->Initialize(input_, audio_);
			CurrentScene_ = m_pScene->GetNextScene();

			break;
		default:
			break;
		}
		// アニメーションの初期化
		gradation_->Initialize(KindAni::EXIT);
		AnimationFrame_ = 0;
	}

	// アウトアニメーション
	if (isOutNow_ && NextScene_ == CurrentScene_) {
		AnimationFrame_++;

		// アニメーション処理
		gradation_->Update(AnimationFrame_, kAnimationFrame_);
		// アニメーション終わった時の処理
		if (AnimationFrame_ == kAnimationFrame_) {
			isInNow_ = false;
			isOutNow_ = false;
			AnimationFrame_ = 0;
			gradation_->SetIsDraw(false);
		}
	}
}
