#include "RestCamera.h"

RestCamera::RestCamera() {}

RestCamera::~RestCamera() {}

void RestCamera::Initialize() {
	// 座標初期化
	pos_ = {0.0f, 0.0f, 0.0f};
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos_;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 3500.0f;
	viewProjection_.Initialize();
}

void RestCamera::Update() {

	// 行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}
