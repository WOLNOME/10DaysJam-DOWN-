#include "enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*> models, const int& enemyType, const Vector3& position) {
	BaseCharacter::Initialize(models);

	enemyType_ = static_cast<EnemyType>(enemyType);

	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemy));
}

void Enemy::Update() { 
	if (enemyType_ == EnemyType::kOne) {

	}
	else if (enemyType_ == EnemyType::kTwo) {

	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	BaseCharacter::Draw(viewProjection);
}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {}

Vector3 Enemy::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}