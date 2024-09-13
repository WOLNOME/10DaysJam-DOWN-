#include "ClearScene.h"
#include "TextureManager.h"
#include "WinApp.h"

ClearScene::ClearScene() {
	// 現在のシーンを設定
	NextScene = Clear;
}

ClearScene::~ClearScene() {}

void ClearScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;

	// スプライトテクスチャ
	textureHandleGameClear_ = TextureManager::Load("Clear/gameClear.png");
	// スプライトの生成
	spriteGameClear_.reset(Sprite::Create(textureHandleGameClear_, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	// モデルの生成

	// インスタンスの生成

	// インスタンスに他クラスを参照
	
	// インスタンス初期化
	

}

void ClearScene::Update() {
	//シーン遷移
	if (input_->TriggerKey(DIK_SPACE)) {
		if (NextScene == Clear) {
			NextScene = Title;
		}
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

	spriteGameClear_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
