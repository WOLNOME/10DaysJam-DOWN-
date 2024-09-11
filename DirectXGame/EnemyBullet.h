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

class player;

class EnemyBullet : public Collider {
public:
	const int32_t kLifeTime = 60 * 5;

	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector3 GetCenter() const override;

	void SetPlayer(player* player) { player_ = player; }

private:
	Model* model_;
	WorldTransform worldTransform_;
	Vector3 velocity_;

	player* player_ = nullptr;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};