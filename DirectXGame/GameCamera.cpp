#include "GameCamera.h"
#include "Script/Matrix.h"

GameCamera::GameCamera() {}

GameCamera::~GameCamera() {}

void GameCamera::Initialize() {
	// 座標初期化
	pos_ = {0.0f,0.0f,0.0f};
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos_;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 3500.0f;
	viewProjection_.Initialize();
	
	//x軸で回転
	worldTransform_.rotation_.x += 1.57f;

}

void GameCamera::Update() {
	// 行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}
