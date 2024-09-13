#include "ClearScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>

ClearScene::ClearScene() {
	// 現在のシーンを設定
	NextScene = Clear;
}

ClearScene::~ClearScene() {}

void ClearScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;
	// ビューポート初期化
	viewProjection_.Initialize();
	
	// スプライトテクスチャ
	textureHandleClearRogo_ = TextureManager::Load("Clear/ClearRogo.png");
	textureHandleSpaceUI_ = TextureManager::Load("Clear/SpaceUI.png");
	textureHandleBack_ = TextureManager::Load("Clear/ClearBack.png");
	// スプライトの生成
	spriteClearRogo_.reset(Sprite::Create(textureHandleClearRogo_, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	spriteSpaceUI_.reset(Sprite::Create(textureHandleSpaceUI_, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 4 * 3}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	spriteBack_.reset(Sprite::Create(textureHandleBack_, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	
	// カーソルを表示
	ShowCursor(true);
}

void ClearScene::Update() {
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	if (input_->TriggerKey(DIK_SPACE)) {
		// タイトルシーンへ遷移
		NextScene = Title;
	}
}

void ClearScene::Draw() {

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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	spriteBack_->Draw();

	spriteClearRogo_->Draw();
	
	spriteSpaceUI_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}