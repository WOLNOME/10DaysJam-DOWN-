#pragma once
#include "cstdint"

enum class CollisionTypeId : std::uint32_t {
	kDefault,
	kPlayer,
	kPlayerBullet,
	kEnemy,
	kEnemyBullet,
};