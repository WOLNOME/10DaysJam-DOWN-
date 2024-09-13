#pragma once
#include "collider.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "cassert"
#include "collisionRecord.h"
#include "collisionTypeIdDef.h"
#include "myFunction.h"
#include <Vector3Function.h>

class PlayerBullet : public Collider {
public:
	const int32_t kLifeTime = 60 * 3;

	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenter() const override;

private:
	Model* model_;
	WorldTransform worldTransform_;
	Vector3 velocity_;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};
