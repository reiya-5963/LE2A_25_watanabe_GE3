#include "Collider.h"

void Collider::Initialize() {
	worldTransform_.Initialize();
}

void Collider::UpdateWorldTransform() {
	worldTransform_.translation_ = GetWorldPosition();
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	model->Draw(worldTransform_, viewProjection);
}

void Collider::SetRadius(const float radius) {
	radius_ = radius;
	worldTransform_.scale_ = {radius_, radius_ , radius_ };
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

uint32_t Collider::GetCollisionAttribute() {
	return 0;
}

void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
}

uint32_t Collider::GetCollisionMask() {
	return 0;
}

void Collider::SetCollisionMask(uint32_t collisionMask) {
	CollisionMask_ = collisionMask;
}
