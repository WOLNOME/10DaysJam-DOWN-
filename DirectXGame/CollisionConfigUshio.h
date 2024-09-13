#pragma once
#include <cstdint>

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
// プレイヤー弾陣営
const uint32_t kCollisionAttributePlayerBullet = 0b1 << 2;
//プレイヤー必殺弾陣営
const uint32_t kCollisionAttributePlayerSpecialBullet = 0b1 << 3;
// 敵弾陣営
const uint32_t kCollisionAttributeEnemyBullet = 0b1 << 4;
// オブジェクト
const uint32_t kCollisionAttributeObject = 0b1 << 5;
//虚無
const uint32_t kCollisionAttributeNothingness = 0b1 << 6;


