#include "TitleScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WinApp.h"
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
	// ビューポート初期化
	viewProjection_.Initialize();
	// ワールドトランスフォーム初期化
	worldTransformObject_.Initialize();
	worldTransformObject_.translation_ = {0.0f, 0.0f, 0.0f};

	// スプライトテクスチャ
	textureHandleTitleRogo_ = TextureManager::Load("Title/titleRogo.png");
	// スプライトの生成
	spriteTitleRogo_.reset(Sprite::Create(textureHandleTitleRogo_, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - 250}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	// モデルの生成
	modelObject_.reset(Model::CreateFromOBJ("titleObject"));
	// インスタンスの生成
	titleCamera_ = make_unique<TitleCamera>();
	titlePlayer_ = make_unique<TitlePlayer>();
	// インスタンス初期化
	titleCamera_->Initialize();
	titlePlayer_->Initialize(input_);
	// カーソルを非表示
	ShowCursor(true);
}

void TitleScene::Update() {

	// カメラの更新処理
	titleCamera_->Update();
	viewProjection_.matView = titleCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// インスタンスの更新処理
	titlePlayer_->Update(viewProjection_);

	/// 遷移アニメーション
	if (titlePlayer_->GetIsStart()) {
		// 開始
		if (!isCameraTransition_ && !isLookHole_ && !isJumpIn_ && !isMoveScene) {
			isCameraTransition_ = true;
		}
		// カメラ遷移
		if (isCameraTransition_) {
			cameraTransitionTimer_++;
			titleCamera_->Transition(titlePlayer_->GetWorldTransformBody(), cameraTransitionTimer_, kCameraTransitionTime_);
			viewProjection_.matView = titleCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の更新と転送
			viewProjection_.TransferMatrix();
			if (cameraTransitionTimer_ >= kCameraTransitionTime_) {
				isCameraTransition_ = false;
				cameraTransitionTimer_ = 0;
				isLookHole_ = true;
			}
		}
		// のぞき処理
		if (isLookHole_) {
			lookHoleTimer_++;
			titleCamera_->LookHole(lookHoleTimer_, kLookHoleTime_);
			viewProjection_.matView = titleCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の更新と転送
			viewProjection_.TransferMatrix();
			if (lookHoleTimer_ >= kLookHoleTime_) {
				isLookHole_ = false;
				lookHoleTimer_ = 0;
				isJumpIn_ = true;
			}
		}
		// ジャンプイン処理
		if (isJumpIn_) {
			jumpInTimer_++;
			titleCamera_->JumpIn(jumpInTimer_);
			viewProjection_.matView = titleCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の更新と転送
			viewProjection_.TransferMatrix();
			if (jumpInTimer_ >= kJumpInTime_) {
				isJumpIn_ = false;
				jumpInTimer_ = 0;
				isMoveScene = true;
				NextScene = Game;
			}
		}
		// 遷移処理中
		if (isMoveScene) {
			titleCamera_->MoveScene();
			viewProjection_.matView = titleCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = titleCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の更新と転送
			viewProjection_.TransferMatrix();
		}
	}

	// ワールドトランスフォーム転送
	worldTransformObject_.UpdateMatrix();

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

	modelObject_->Draw(worldTransformObject_, viewProjection_);
	if (!titlePlayer_->GetIsStart() || isCameraTransition_) {
		titlePlayer_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (!titlePlayer_->GetIsStart()) {
		titlePlayer_->DrawUI();
		spriteTitleRogo_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
