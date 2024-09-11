#include "enemy.h"
#include "Player.h"
#include "ImGuiManager.h"

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
		Vector3 EnemyToPlayer = GetCenter() - player_->GetCenter();

		EnemyToPlayer.Normalize();

		worldTransform_.translation_ -= EnemyToPlayer * speed_;

	}
	else if (enemyType_ == EnemyType::kTwo) {

	}

	worldTransform_.UpdateMatrix();

	#ifdef _DEBUG
	ImGui::Begin("Enemy");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);

	ImGui::End();


	#endif _DEBUG
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手がプレイヤーの場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayer)) {
		isDead_ = true;
	}
}

Vector3 Enemy::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}