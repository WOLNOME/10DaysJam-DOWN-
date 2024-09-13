#include "CollisionManagerUshio.h"
#include "CollisionConfigUshio.h"
#include "Script/MyTools.h"

void CollisionManagerUshio::CheckCollision() {
	// リスト内のペアを総当たり
	std::list<ColliderUshio*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		ColliderUshio* colliderA = *itrA;
		// イテレータBはイテレーターAの次の要素から回す
		std::list<ColliderUshio*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			// イテレーターBからコライダーBを取得する
			ColliderUshio* colliderB = *itrB;
			// 衝突フィルタリング
			// 自分と同じ属性だと当たらない
			if (colliderA->GetCollisionAttribute() == colliderB->GetCollisionAttribute()) {
				continue;
			}
			// 自機と自弾&必殺弾
			if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayerBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayerBullet) {
					continue;
				}
			}
			if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					continue;
				}
			}
			// 敵と敵弾
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemyBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemyBullet) {
					continue;
				}
			}
			// オブジェクトとプレイヤー以外
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayerBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayerBullet) {
					continue;
				}
			}
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemy) {
					continue;
				}
			}
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemyBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemyBullet) {
					continue;
				}
			}
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeObject) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					continue;
				}
			}

			// 虚無オブジェクトは当たり判定を取らない
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeNothingness) {
				if (colliderB->GetCollisionAttribute() > 0) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeNothingness) {
				if (colliderA->GetCollisionAttribute() > 0) {
					continue;
				}
			}

			// プレイヤーとオブジェクトの特殊当たり判定
			if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributeObject) {
					CheckSpecialCollisionPair(colliderA, colliderB);
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayer) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributeObject) {
					CheckSpecialCollisionPair(colliderA, colliderB);
					continue;
				}
			}

			// 敵と必殺弾の特殊当たり判定
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					CheckSpecialCollisionPair(colliderA, colliderB);
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayerSpecialBullet) {
					CheckSpecialCollisionPair(colliderA, colliderB);
					continue;
				}
			}

			//敵と自機の特殊当たり判定
			if (colliderA->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderB->GetCollisionAttribute() == kCollisionAttributePlayer) {
					CheckPAECollisionPair(colliderB, colliderA);
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
				if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayer) {
					CheckPAECollisionPair(colliderA, colliderB);
					continue;
				}
			}

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManagerUshio::CheckCollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB) {
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();
	// ABの差
	Vector3 AtoB = MyTools::Subtract(posA, posB);
	// 半径の和
	float addRad = colliderA->GetRadius() + colliderB->GetRadius();
	// 座標AとBの距離を求める
	float length = MyTools::Length(AtoB);
	// 球と球の交差判定
	if (length < addRad) {
		// 衝突時コールバックを呼び出す
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void CollisionManagerUshio::CheckSpecialCollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB) {
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();
	// ABの差
	Vector3 AtoB = MyTools::Subtract(posA, posB);
	// 半径の和
	float addRad = colliderA->GetRadius() + colliderB->GetRadius();
	// 座標AとBの距離を求める
	float length = MyTools::Length(AtoB);
	// 球と球の交差判定
	if (length < addRad) {
		// 衝突時コールバックを呼び出す
		colliderA->OnSpecialCollision();
		colliderB->OnSpecialCollision();
	}
}

void CollisionManagerUshio::CheckPAECollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB) {
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();
	// ABの差
	Vector3 AtoB = MyTools::Subtract(posA, posB);
	// 半径の和
	float addRad = colliderA->GetRadius() + colliderB->GetRadius();
	// 座標AとBの距離を求める
	float length = MyTools::Length(AtoB);
	// 球と球の交差判定
	if (length < addRad) {
		// Aのコールバックのみ呼び出す
		colliderA->OnCollision();
	}
}
