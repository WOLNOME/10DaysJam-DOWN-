#define _USE_MATH_DEFINES
#include "Player.h"
#include "ImGuiManager.h"
#include "Wall.h"
#include <cmath>
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

	SetCursorPos(int(mouseCenter_.x), int(mouseCenter_.y));

	kCharacterSpeed_ = 0.5f;
	mouseMove_ = {0.0f, 0.0f};
	fallingVelocity_ = {0.0f, 8.0f, 0.0f};
	fallingVelocityJet_ = {0.0f, 3.0f, 0.0f};

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kPlayer));

	// プレイヤーの半径
	Collider::SetRadius(1.0f);
}

/// 更新
void Player::Update(ViewProjection& viewProjection) {
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

	ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);   // ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);

	const float kMoveLimitX = 48.0f;
	const float kMoveLimitZ = 48.0f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);
	// 移動を消す
	worldTransform_.translation_.y = 0;

	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
	// 回転を消す
	worldTransform_.rotation_.z = 0.0f;

	ImGui::DragFloat2("mouseMove", &mouseMove_.x, 0.05f);
	ImGui::DragFloat2("mousePos", &mousePos_.x, 0.05f);

	ImGui::Text("Push P Key : mouseLock switching ");
	ImGui::Checkbox("Debug", &isDebug_);

	ImGui::DragFloat3("fallVelocity", &fallingVelocity_.x, 0.05f);
	ImGui::DragFloat3("fallVelocityJet", &fallingVelocityJet_.x, 0.05f);

	ImGui::End();

#endif // _DEBUG

	// 行列の更新・転送
	worldTransform_.UpdateMatrix();

	WorldTo3DReticle();
	Reticle3Dto2D(viewProjection);

	// ジェットパックの処理
	if (input_->TriggerKey(DIK_SPACE) || input_->PushKey(DIK_SPACE)) {
		wall_->SetFallVelocity(fallingVelocityJet_);
	}
	else
	{
		wall_->SetFallVelocity(fallingVelocity_);
	}

}

/// 移動処理
void Player::Translation() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キーボード操作
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed_;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed_;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed_;
	} else if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed_;
	}

	// 移動ベクトルの向きを自キャラの向きに合わせる
	Matrix4x4 rotationMatrix = Matrix::MakeRotateMatrix4x4(0.0f, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	move = Matrix::Multiply(move, rotationMatrix);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, move);

	const float kMoveLimitX = 48.0f;
	const float kMoveLimitZ = 48.0f;

	// 制限
	worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);

	// 移動を消す
	worldTransform_.translation_.y = 0;
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
	mousePos_ = {float(mouse.x), float(mouse.y)};

	mouseMove_ = {0.0f, 0.0f};
	mouseMove_ = MyTools::Normalize(float(mousePos_.x) - WinApp::kWindowWidth / 2, float(mousePos_.y) - WinApp::kWindowHeight / 2)*(3.0f/60.0f);

	
	SetCursorPos(int(mouseCenter_.x), int(mouseCenter_.y));

	worldTransform_.rotation_.x += mouseMove_.y;
	worldTransform_.rotation_.y += mouseMove_.x;

	// 制限
	worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
}

/// ワールド座標から3Dレティクルの座標を計算
void Player::WorldTo3DReticle() {
	// 自機から3Dレティクルの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = Matrix::TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = MyTools::Multiply(kDistancePlayerTo3DReticle, MyTools::Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = MyTools::Add(GetCenter(), offset);
	worldTransform3DReticle_.UpdateMatrix();
}

/// 3Dレティクルの座標から2Dレティクルの座標を計算
void Player::Reticle3Dto2D(ViewProjection& viewProjection) {
	Vector3 positionReticle = Get3DReticleWorldPosition();

	// ビューポート行列
	Matrix4x4 matViewport = Matrix::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Matrix::Multiply(Matrix::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Matrix::Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

Vector3 Player::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

/// 3Dレティクルのワールド座標を取得
Vector3 Player::Get3DReticleWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

/// 描画
void Player::Draw(ViewProjection& viewProjection) { 
	BaseCharacter::Draw(viewProjection); 
}

void Player::DrawUI() {
	// 2Dレティクルを描画
	sprite2DReticle_->Draw();
}
