#define _USE_MATH_DEFINES
#include "Player.h"
#include "ImGuiManager.h"
#include "Wall.h"
#include <cmath>
#include <cassert>

Player::~Player() {
	//削除
	bullets_.clear();
}

/// 初期化
void Player::Initialize(const std::vector<Model*> models) {
	// 3Dモデルの生成
	BaseCharacter::Initialize(models);
	bulletModel_.reset(Model::CreateFromOBJ("player"));

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

	// リロード時間をセット
	reLoadTimer_ = 0.0f;

	// 体力セット
	hp_ = kMaxHp_;

	isDead_ = false;
}

/// 更新
void Player::Update(ViewProjection& viewProjection) {
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

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

	//ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);   // ウィンドウの座標(プログラム起動時のみ読み込み)
	//ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Player");

	//ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);

	//const float kMoveLimitX = 48.0f;
	//const float kMoveLimitZ = 48.0f;

	//// 制限
	//worldTransform_.translation_.x = MyTools::Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	//worldTransform_.translation_.z = MyTools::Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitZ);
	//// 移動を消す
	//worldTransform_.translation_.y = 0;

	//ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);

	//// 制限
	//worldTransform_.rotation_.x = MyTools::Clamp(worldTransform_.rotation_.x, 0.0f, float(M_PI_2));
	//// 回転を消す
	//worldTransform_.rotation_.z = 0.0f;

	//ImGui::DragFloat2("mouseMove", &mouseMove_.x, 0.05f);
	//ImGui::DragFloat2("mousePos", &mousePos_.x, 0.05f);

	//ImGui::Text("Push P Key : mouseLock switching ");
	//ImGui::Checkbox("Debug", &isDebug_);

	//ImGui::DragFloat3("fallVelocity", &fallingVelocity_.x, 0.05f);
	//ImGui::DragFloat3("fallVelocityJet", &fallingVelocityJet_.x, 0.05f);

	//ImGui::DragFloat("ReLoadTimer", &reLoadTimer_, 0.05f);

	ImGui::Text("HP : %d", hp_);
	ImGui::Checkbox("isDead", &isDead_);

	ImGui::End();

#endif // _DEBUG

	// 行列の更新・転送
	worldTransform_.UpdateMatrix();

	WorldTo3DReticle();
	Reticle3Dto2D(viewProjection);

	// 攻撃処理
	Attack();

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

/// 攻撃
void Player::Attack() {
	// リロードが終わっている時
	if (reLoadTimer_ <= 0) {
		// 左クリックが押されている時
		if (input_->IsPressMouse(0)) {
			// 弾発射
			Fire();

			// リロード時間をセット
			reLoadTimer_ = kReLoadTime_;
		}
	}

	// リロード時間の経過処理
	reLoadTimer_--;
}

/// 弾発射
void Player::Fire() {
	// 弾の速度
	float speed = 0.5f;
	// レティクルの方向を取得(正規化済み)
	Vector3 reticleDir = MyTools::Normalize(Get3DReticleWorldPosition() - GetCenter());
	
	// 弾の生成
	CreateBullet(bulletModel_.get(), GetCenter(), reticleDir * speed);
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

/// 弾の生成
void Player::CreateBullet(Model* model, const Vector3& pos, const Vector3& velocity) {
	// 新しい弾の生成
	PlayerBullet* bullet = new PlayerBullet();

	bullet->Initialize(model, pos, velocity);

	// 弾をリストにセット
	bullets_.push_back(bullet);
}

/// 衝突判定
void Player::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵 又は 敵の弾の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kEnemy) || typeID == static_cast<uint32_t>(CollisionTypeId::kEnemyBullet)) {
		// 体力減少
		hp_--;

		// 体力がなくなった時
		if (hp_ <= 0) {
			isDead_ = true;
			return;
		}
	}	
}

/// 描画
void Player::Draw(ViewProjection& viewProjection) { 
	BaseCharacter::Draw(viewProjection); 

	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() {
	// 2Dレティクルを描画
	sprite2DReticle_->Draw();
}
