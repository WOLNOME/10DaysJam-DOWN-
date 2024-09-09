#define _USE_MATH_DEFINES
#include "Player.h"
#include <cmath>
#include "ImGuiManager.h"
#include <cassert>

/// 初期化
void Player::Initialize(const std::vector<Model*> models) {
	// 3Dモデルの生成
	BaseCharacter::Initialize(models);

	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.UpdateMatrix();

	uint32_t reticleTexture = TextureManager::Load("reticle.png");
	sprite2DReticle_ = Sprite::Create(reticleTexture, {500, 100}, Vector4(1.0f, 1.0f, 1.0f, 1.0f), {0.5f, 0.5f});

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.f, 0.f, 0.f};
	worldTransform_.rotation_ = {1.57f, 0.0f, 0.0f};
	worldTransform_.UpdateMatrix();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));
}

/// 更新
void Player::Update() {
	MouseMove();

	// 移動処理
	Translation();

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat2("mouseMove", &mouseMove.x, 0.05f);
	ImGui::DragFloat2("mousePos", &mousePos.x, 0.05f);

	ImGui::End();

#endif // _DEBUG


}

/// 移動処理
void Player::Translation() {
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
	Matrix4x4 rotationMatrix = Matrix::MakeRotateMatrix4x4(worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	move = Matrix::Multiply(move, rotationMatrix);

	move.y = 0;

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, move);

	const float kMoveLimitX = 48.0f;
	const float kMoveLimitZ = 48.0f;

	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);



}

/// 旋回処理
void Player::Rotate() { 
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	
	// 押した方向でベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.z += kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.z -= kRotSpeed;
	}
}

/// マウスでの視点移動
void Player::MouseMove() {
	// マウス座標（スクリーン座標）を取得
	POINT mouse;
	GetCursorPos(&mouse);
	mousePos = {float(mouse.x), float(mouse.y)};

	mouseMove = {0.0f, 0.0f};
	mouseMove = MyTools::Normalize(float(mousePos.x) - WinApp::kWindowWidth / 2, float(mousePos.y) - WinApp::kWindowHeight / 2)*(3.0f/60.0f);

	
	SetCursorPos(int(mouseCenter.x), int(mouseCenter.y));

	worldTransform_.rotation_.x += mouseMove.y;
	worldTransform_.rotation_.y += mouseMove.x;

	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
	worldTransform_.UpdateMatrix();
}

Vector3 Player::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

/// 描画
void Player::Draw(ViewProjection& viewProjection) { 
	BaseCharacter::Draw(viewProjection); 
}
