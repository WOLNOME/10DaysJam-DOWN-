#include "RestItem.h"
#include "CollisionConfigUshio.h"

RestItem::RestItem() {}

RestItem::~RestItem() {}

void RestItem::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.translation_ = prePos_;

	modelHeart_.reset(Model::CreateFromOBJ("heart"));

	radius_ = 2.0f;

	SetCollisionAttribute(kCollisionAttributeObject);
}

void RestItem::Update() {
	if (worldTransform_.translation_.y > 2.8f || worldTransform_.translation_.y < 2.2f) {
		velocity_.y = -velocity_.y;
	}
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.rotation_.y += 0.03f;

	worldTransform_.UpdateMatrix();
}

void RestItem::Draw(ViewProjection& viewProjection) { modelHeart_->Draw(worldTransform_, viewProjection); }

void RestItem::OnCollision() {}

void RestItem::OnSpecialCollision() { isDead_ = true; }

Vector3 RestItem::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
