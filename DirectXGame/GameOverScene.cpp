#include "GameOverScene.h"
#include "TextureManager.h"
#include "WinApp.h"

GameOverScene::GameOverScene() {
	// 現在のシーンを設定
	NextScene = GameOver;
}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;

	// スプライトテクスチャ
	textureHandleGameOver_ = TextureManager::Load("gameOver/gameOver.png");
	// スプライトの生成
	spriteGameOver_.reset(Sprite::Create(textureHandleGameOver_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	// モデルの生成

	// インスタンスの生成

	// インスタンスに他クラスを参照

	// インスタンス初期化
}

void GameOverScene::Update() {
	// シーン遷移
	if (input_->TriggerKey(DIK_SPACE)) {
		if (NextScene == GameOver) {
			NextScene = Title;
		}
	}
}

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// モデル

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	spriteGameOver_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
