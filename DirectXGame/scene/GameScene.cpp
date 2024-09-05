#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() {
	//現在のシーンを設定
	NextScene = Game;
}

GameScene::~GameScene() {}

void GameScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// スプライトテクスチャ

	// スプライトの生成
	
	// モデルテクスチャ

	// モデルの生成

	// インスタンスの生成
	gameCamera_ = std::make_unique<GameCamera>();
	wall_ = std::make_unique<Wall>();

	// インスタンス初期化
	gameCamera_->Initialize();
	wall_->Initialize();
	



}

void GameScene::Update() {
	// カメラの更新処理
	gameCamera_->Update();
	viewProjection_.matView = gameCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = gameCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	//オブジェクトの更新処理
	wall_->Update();















#ifdef _DEBUG
	ImGui::Begin("GameSceneNow");

	ImGui::End();
#endif // _DEBUG
}

void GameScene::Draw() {

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

	//モデル
	wall_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
