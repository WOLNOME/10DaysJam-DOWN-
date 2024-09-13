#include "RestScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>

RestScene::RestScene() {
	// 現在のシーンを設定
	NextScene = Rest;
}

RestScene::~RestScene() {}

void RestScene::Initialize(Input* input, Audio* audio) {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = input;
	audio_ = audio;
	// ビューポート初期化
	viewProjection_.Initialize();
	// ワールドトランスフォーム初期化
	worldTransformBeforeObject_.Initialize();
	worldTransformBeforeObject_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformAfterObject_.Initialize();
	worldTransformAfterObject_.translation_ = {0.0f, 0.0f, 0.0f};

	// スプライトテクスチャ

	// スプライトの生成

	// モデルの生成
	modelBeforeObject_.reset(Model::CreateFromOBJ("restBeforeObject"));
	modelAfterObject_.reset(Model::CreateFromOBJ("restAfterObject"));

	// インスタンスの生成
	restCamera_ = make_unique<RestCamera>();
	restPlayer_ = make_unique<RestPlayer>();
	restItem_ = make_unique<RestItem>();
	collisionManager_ = make_unique<CollisionManagerUshio>();

	// インスタンスに他クラスを参照
	restCamera_->SetParent(&restPlayer_->GetWorldTransform());

	// インスタンス初期化
	restCamera_->Initialize();
	restPlayer_->Initialize();
	restItem_->Initialize();

	// カーソルを非表示
	ShowCursor(true);
}

void RestScene::Update() {

	// プレイヤー処理
	restPlayer_->Update();

	// カメラ処理
	restCamera_->Update();
	viewProjection_.matView = restCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = restCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の更新と転送
	viewProjection_.TransferMatrix();

	// インスタンス処理
	if (!restItem_->GetIsDead()) {
		restItem_->Update();
	}
	//プレイヤーから獲得判定を得る
	if (!isEnhanced) {
		isEnhanced = restPlayer_->GetIsGetItem();
	}

	// コリジョン
	CheckAllCollision();

	// プレイヤーから遷移フラグを受け取る
	if (restPlayer_->GetIsTransition()) {
		if (NextScene == Rest) {
			NextScene = Game;
		}
	}

}

void RestScene::Draw() {

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
	if (!isEnhanced) {
		modelBeforeObject_->Draw(worldTransformBeforeObject_, viewProjection_);
	} else {
		modelAfterObject_->Draw(worldTransformAfterObject_, viewProjection_);
	}
	if (!restItem_->GetIsDead()) {
		restItem_->Draw(viewProjection_);
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
	
	restPlayer_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void RestScene::CheckAllCollision() {
	// 衝突マネージャーのクリア
	collisionManager_->ClearColliders();

	// コライダー
	std::list<ColliderUshio*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(restPlayer_.get());
	if (!restItem_->GetIsDead()) {
		colliders_.push_back(restItem_.get());
	}

	// 衝突マネージャーのリストにコライダーを登録する
	collisionManager_->SetColliders(colliders_);
	// 衝突判定の当たり判定処理を呼び出す
	collisionManager_->CheckCollision();
}
