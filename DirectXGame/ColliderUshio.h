#pragma once
#include "Vector3.h"
#include <cstdint>
class ColliderUshio {

public:
	virtual ~ColliderUshio() = default;
	virtual float GetRadius() { return radius_; }
	virtual void OnCollision();
	virtual void OnSpecialCollision();
	virtual Vector3 GetWorldPosition() = 0;
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(const uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

private:
	uint32_t collisionAttribute_ = 0xffffffff;

protected:
	float radius_;
};
