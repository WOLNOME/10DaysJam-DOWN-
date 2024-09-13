#pragma once
#include <list>
#include "ColliderUshio.h"

class CollisionManagerUshio {
public:
	// 当たり判定チェック
	void CheckCollision();
	// コライダーリスト登録関数
	void SetColliders(std::list<ColliderUshio*> colliders) { colliders_ = colliders; }
	// コライダーリストクリア関数
	void ClearColliders() { colliders_.clear(); }

private:
	// 当たり判定演算
	void CheckCollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB);
	// 特殊当たり判定チェック
	void CheckSpecialCollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB);
	//敵属性と自機属性の衝突判定
	void CheckPAECollisionPair(ColliderUshio* colliderA, ColliderUshio* colliderB);

	// コライダーリスト
	std::list<ColliderUshio*> colliders_;
};
