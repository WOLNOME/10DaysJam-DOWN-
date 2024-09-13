#include "enemy.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "Script/Player.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*> models, const int& enemyType, const Vector3& position) {
	BaseCharacter::Initialize(models);

	enemyType_ = static_cast<EnemyType>(enemyType);

	worldTransform_.translation_ = position;

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemy));

	if (enemyType_ == EnemyType::kFollow) {
		speed_ = 0.01f;

		Vector3 scale = {1.0f, 1.0f, 1.0f};
		worldTransform_.scale_ = scale;

		Collider::SetRadius(1.0f);

	} else if (enemyType_ == EnemyType::kFront) {
		speed_ = 0.15f;
		Vector3 offset = {0.0f, -150.0f, -47.0f};
		worldTransform_.translation_ = offset;

		Vector3 scale = {2.0f, 2.0f, 2.0f};
		worldTransform_.scale_ = scale;

		Collider::SetRadius(2.0f);

	} else if (enemyType_ == EnemyType::kBack) {
		speed_ = 0.15f;
		Vector3 offset = {0.0f, -150.0f, 47.0f};
		worldTransform_.translation_ = offset;

		Vector3 scale = {2.0f, 2.0f, 2.0f};
		worldTransform_.scale_ = scale;

		Collider::SetRadius(2.0f);

	} else if (enemyType_ == EnemyType::kRight) {
		speed_ = 0.15f;
		Vector3 offset = {47.0f, -150.0f, 0.0f};
		worldTransform_.translation_ = offset;

		Vector3 scale = {2.0f, 2.0f, 2.0f};
		worldTransform_.scale_ = scale;

		Collider::SetRadius(2.0f);

	} else if (enemyType_ == EnemyType::kLeft) {
		speed_ = 0.15f;
		Vector3 offset = {-47.0f, -150.0f, 0.0f};
		worldTransform_.translation_ = offset;

		Vector3 scale = {2.0f, 2.0f, 2.0f};
		worldTransform_.scale_ = scale;

		Collider::SetRadius(2.0f);
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Update() {

	if (enemyType_ == EnemyType::kFollow) {
		float followSpeed = 0.03f;

		velocity_ = GetCenter() - player_->GetCenter();

		velocity_.Normalize();

		worldTransform_.translation_ -= velocity_ * followSpeed;
	}

	if (enemyType_ == EnemyType::kFront) {
		if (worldTransform_.translation_.y < -25.0f) {
			worldTransform_.translation_.y += 0.5f;
		}


		if (randTimer_ > 0) {
			randTimer_--;

			if (randIndex_ == 0) {
				if (worldTransform_.translation_.x < 48.0f) {
					velocity_ = {1.0f, 0.0f, 0.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 1;
				}
			} else {
				if (worldTransform_.translation_.x > -48.0f) {
					velocity_ = {-1.0f, 0.0f, 0.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 0;
				}
			}
		} else {
			// 10~50のランダムな値を取得
			float randTime = Rand(30.0f, 120.0f);
			randTimer_ = int(std::floor(randTime));

			// 0か1のランダムな値を取得
			rand_ = Rand(0.0f, 2.0f);
			randIndex_ = int(std::floor(rand_));

			Fire();
		}
	}

	if (enemyType_ == EnemyType::kBack) {
		if (worldTransform_.translation_.y < -25.0f) {
			worldTransform_.translation_.y += 0.5f;
		}


		if (randTimer_ > 0) {
			randTimer_--;

			if (randIndex_ == 0) {
				if (worldTransform_.translation_.x < 48.0f) {
					velocity_ = {1.0f, 0.0f, 0.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 1;
				}
			} else {
				if (worldTransform_.translation_.x > -48.0f) {
					velocity_ = {-1.0f, 0.0f, 0.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 0;
				}
			}
		} else {
			// 10~50のランダムな値を取得
			float randTime = Rand(30.0f, 120.0f);
			randTimer_ = int(std::floor(randTime));

			// 0か1のランダムな値を取得
			rand_ = Rand(0.0f, 2.0f);
			randIndex_ = int(std::floor(rand_));

			Fire();
		}
	}

	if (enemyType_ == EnemyType::kRight) {
		if (worldTransform_.translation_.y < -25.0f) {
			worldTransform_.translation_.y += 0.5f;
		}


		if (randTimer_ > 0) {
			randTimer_--;

			if (randIndex_ == 0) {
				if (worldTransform_.translation_.z < 48.0f) {
					velocity_ = {0.0f, 0.0f, 1.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 1;
				}
			} else {
				if (worldTransform_.translation_.z > -48.0f) {
					velocity_ = {0.0f, 0.0f, -1.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 0;
				}
			}
		} else {
			// 10~50のランダムな値を取得
			float randTime = Rand(30.0f, 120.0f);
			randTimer_ = int(std::floor(randTime));

			// 0か1のランダムな値を取得
			rand_ = Rand(0.0f, 2.0f);
			randIndex_ = int(std::floor(rand_));

			Fire();
		}
	}

	if (enemyType_ == EnemyType::kLeft) {
		if (worldTransform_.translation_.y < -25.0f) {
			worldTransform_.translation_.y += 0.5f;
		}


		if (randTimer_ > 0) {
			randTimer_--;

			if (randIndex_ == 0) {
				if (worldTransform_.translation_.z < 48.0f) {
					velocity_ = {0.0f, 0.0f, 1.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 1;
				}
			} else {
				if (worldTransform_.translation_.z > -48.0f) {
					velocity_ = {0.0f, 0.0f, -1.0f};
					worldTransform_.translation_ += velocity_ * speed_;
				} else {
					randIndex_ = 0;
				}
			}
		} else {
			// 10~50のランダムな値を取得
			float randTime = Rand(30.0f, 120.0f);
			randTimer_ = int(std::floor(randTime));

			// 0か1のランダムな値を取得
			rand_ = Rand(0.0f, 2.0f);
			randIndex_ = int(std::floor(rand_));

			Fire();
		}
	}

	// 弾の更新
	// for (auto& bullet : bullets_) {
	//	bullet->Update();
	//}

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Enemy");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.05f);
	ImGui::Checkbox("IsDead", &isDead_);

	ImGui::End();

#endif _DEBUG
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransform_, viewProjection);

	// for (auto& bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	// }
}

void Enemy::Fire() {
	float speed = 0.01f;
	Vector3 bulletPos = GetCenter();
	Vector3 bulletVelocity = player_->GetCenter() - GetCenter();

	bulletVelocity.Normalize();

	gameScene_->CreateEnemyBullet(models_[0], bulletPos, bulletVelocity * speed);
}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手がプレイヤーの場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayer)) {
		if (enemyType_ == EnemyType::kFollow) {
			isDead_ = true;
		}
	}

	// 衝突相手がプレイヤーの弾の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayerBullet)) {
		isDead_ = true;
	}

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);

		if (enemyType_ == EnemyType::kFollow) {
			velocity_ = (GetCenter() - enemy->GetCenter()).Normalize();
		}

		if (enemyType_ == EnemyType::kFront) {
			if (randIndex_ == 0) {
				randIndex_ = 1;
			}
		}

		if (enemyType_ == EnemyType::kBack) {
			if (randIndex_ == 0) {
				randIndex_ = 1;
			}
		}

		if (enemyType_ == EnemyType::kRight) {
			if (randIndex_ == 0) {
				randIndex_ = 1;
			}
		}

		if (enemyType_ == EnemyType::kLeft) {
			if (randIndex_ == 0) {
				randIndex_ = 1;
			}
		}
	}
}

Vector3 Enemy::GetCenter() const {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}