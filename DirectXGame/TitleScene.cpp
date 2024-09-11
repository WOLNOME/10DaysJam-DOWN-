#include "TitleScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

TitleScene::TitleScene() {
	// 現在のシーンを設定
	NextScene = Title;
}

TitleScene::~TitleScene() {}

void TitleScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;

	// スプライトテクスチャ
	textureHandleTitle_ = TextureManager::Load("title.png");
	// スプライトの生成
	spriteTitle_.reset(Sprite::Create(textureHandleTitle_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f}));
	// モデルテクスチャ

	// モデルの生成
	
	// インスタンスの生成
	
	// インスタンス初期化
	
	// カーソルを非表示
	ShowCursor(false);
}

void TitleScene::Update() {
	//シーン遷移
	if (input_->TriggerKey(DIK_SPACE)) {
		NextScene = Game;
	}







#ifdef _DEBUG
	ImGui::Begin("TitleSceneNow");

	ImGui::End();
#endif // _DEBUG
}

void TitleScene::Draw() {

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

	spriteTitle_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
