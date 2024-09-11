#include "EnemyBullet.h"
#include "player.h"

EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	
	Vector3 scale = {0.5f, 0.5f, 0.5f};
	worldTransform_.scale_ = scale;

	velocity_ = velocity;

	worldTransform_.UpdateMatrix();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemyBullet));

	Collider::SetRadius(0.5f);
}

void EnemyBullet::Update() {
	if (isDead_) {
		return;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	deathTimer_--;
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	if (isDead_) {
		return;
	}

	model_->Draw(worldTransform_, viewProjection);
}

void EnemyBullet::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手がプレイヤーの場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayer)) {
		isDead_ = true;
	}
}

Vector3 EnemyBullet::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
