#include "TitleCamera.h"
#include "ImGuiManager.h"
#include "Script/MyTools.h"

TitleCamera::TitleCamera() {}

TitleCamera::~TitleCamera() {}

void TitleCamera::Initialize() {
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	position_ = {0.0, 14.38f, -18.03f};
	rotation_ = {0.59f, 0.0f, 0.0f};
	worldTransform_.translation_ = position_;
	worldTransform_.rotation_ = rotation_;
	startPosition_ = position_;
	startRotation_ = rotation_;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();
}

void TitleCamera::Update() {
	// 行列の更新と転送
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("position", &position_.x, 0.01f);
	ImGui::DragFloat3("rotation", &rotation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void TitleCamera::Transition(WorldTransform& worldTransform, int timer, int kTime) {
	// 移動処理(イージング)
	goalPosition_ = worldTransform.translation_;
	goalPosition_.y = worldTransform.translation_.y + 1.5f;
	worldTransform_.translation_ = MyTools::Lerp(startPosition_, goalPosition_, float((float)timer / kTime));

	// 回転処理(イージング)
	goalRotation_ = worldTransform.rotation_;
	worldTransform_.rotation_ = MyTools::Lerp(startRotation_, goalRotation_, float((float)timer / kTime));

	worldTransform_.UpdateMatrix();
	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}

void TitleCamera::LookHole(int timer, int kTime) {
	//
	Vector3 goalDirection;
	if (timer == 1) {
		startRotation_ = worldTransform_.rotation_;
		goalDirection = MyTools::Subtract({0.0f, -7.0f, 0.0f}, worldTransform_.translation_);
		goalRotation_.y = std::atan2(goalDirection.x, goalDirection.z);
		Matrix4x4 rotateYMatrix = Matrix::MakeRotateYMatrix4x4(-goalRotation_.y);
		Vector3 velocityZ = Matrix::Transform(goalDirection, rotateYMatrix);
		goalRotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	}

	worldTransform_.rotation_ = MyTools::Lerp(startRotation_, goalRotation_, float((float)timer / kTime));

	worldTransform_.UpdateMatrix();
	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}

void TitleCamera::JumpIn(int timer) {
	if (timer == 1) {
		// velocityを決める
		velocity_.x = MyTools::Normalize(MyTools::Subtract({0.0f, 0.0f, 0.0f}, worldTransform_.translation_)).x * kSpeed;
		velocity_.z = MyTools::Normalize(MyTools::Subtract({0.0f, 0.0f, 0.0f}, worldTransform_.translation_)).z * kSpeed;
		velocity_.y = kJumpHeight_;
	}
	// 重力加算
	velocity_.y += -kGravity_;
	// 速度加算
	worldTransform_.translation_ += velocity_;
	position_ = worldTransform_.translation_;
	rotation_ = worldTransform_.rotation_;
	worldTransform_.UpdateMatrix();
	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}

void TitleCamera::MoveScene() { 
	worldTransform_.translation_ = position_;
	worldTransform_.rotation_ = rotation_;
	worldTransform_.UpdateMatrix();
	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::Inverse(worldTransform_.matWorld_);
}
