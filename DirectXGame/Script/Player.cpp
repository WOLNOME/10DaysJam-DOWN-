#include "Player.h"
#include <cassert>

/// 初期化
void Player::Initialize(const std::vector<Model*> models) {
	// 3Dモデルの生成
	BaseCharacter::Initialize(models);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.f, 0.f, 0.f};
	worldTransform_.UpdateMatrix();
	
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

/// 更新
void Player::Update() {
	// 旋回処理
	Rotate();

	// 移動処理
	Translation();

	worldTransform_.UpdateMatrix();

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

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = MyTools::Add(worldTransform_.translation_, move);
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
