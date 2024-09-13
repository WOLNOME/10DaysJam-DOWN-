#include "RestItem.h"

RestItem::RestItem() {}

RestItem::~RestItem() {}

void RestItem::Initialize() {}

void RestItem::Update() {}

void RestItem::Translation() {}

void RestItem::Rotate() {}

void RestItem::MouseMove() {}

Vector3 RestItem::GetCenter() const { return Vector3(); }

void RestItem::Draw(ViewProjection& viewProjection) {}

void RestItem::OnCollision(Collider* other) {}
