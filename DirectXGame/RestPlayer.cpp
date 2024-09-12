#define _USE_MATH_DEFINES
#include "RestPlayer.h"
#include <cmath>
#include "ImGuiManager.h"
#include <cassert>

RestPlayer::RestPlayer() {}

RestPlayer::~RestPlayer() {}

void RestPlayer::Initialize() {
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.f, playerHeight_, 0.f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.UpdateMatrix();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));
}

void RestPlayer::Update() {
	if (!isDebug_) {
		// マウスによる視点移動
		MouseMove();
	}

	// 移動処理
	Translation();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebug_ = !isDebug_;
	}
	ShowCursor(isDebug_);

	ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);    // ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);

	const float kMoveLimitX = 9.5f;
	const float kMoveLimitZ = 9.5f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);
	// 移動を消す
	worldTransform_.translation_.y = playerHeight_;

	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
	// 回転を消す
	worldTransform_.rotation_.z = 0.0f;

	ImGui::DragFloat2("mouseMove", &mouseMove.x, 0.05f);
	ImGui::DragFloat2("mousePos", &mousePos.x, 0.05f);

	ImGui::Text("Push P Key : mouseLock switching ");
	ImGui::Checkbox("Debug", &isDebug_);

	ImGui::End();

#endif // _DEBUG

	// 行列の更新・転送
	worldTransform_.UpdateMatrix();
}

void RestPlayer::Translation() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キーボード操作
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	}

	// 移動ベクトルの向きを自キャラの向きに合わせる
	Matrix4x4 rotationMatrix = Matrix::MakeRotateMatrix4x4(0.0f, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	move = Matrix::Multiply(move, rotationMatrix);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, move);

	const float kMoveLimitX = 9.5f;
	const float kMoveLimitZ = 9.5f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);

	// 移動を消す
	worldTransform_.translation_.y = playerHeight_;
}

void RestPlayer::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向でベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.z += kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.z -= kRotSpeed;
	}
}

void RestPlayer::MouseMove() {
	// マウス座標（スクリーン座標）を取得
	POINT mouse;
	GetCursorPos(&mouse);
	mousePos = {float(mouse.x), float(mouse.y)};

	mouseMove = {0.0f, 0.0f};
	mouseMove = MyTools::Normalize(float(mousePos.x) - WinApp::kWindowWidth / 2, float(mousePos.y) - WinApp::kWindowHeight / 2) * (3.0f / 60.0f);

	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));

	worldTransform_.rotation_.x += mouseMove.y;
	worldTransform_.rotation_.y += mouseMove.x;

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
}

Vector3 RestPlayer::GetCenter() const {
	Vector3 worldPos;
	
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void RestPlayer::Draw(ViewProjection& viewProjection) { viewProjection; }

void RestPlayer::OnCollision(Collider* other) { other; }
