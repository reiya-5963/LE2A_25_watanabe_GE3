#include "Ground.h"

void Ground::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTrans_.Initialize();
	worldTrans_.translation_ = position;

	Collider::Initialize();
	SetRadius({ 60.0f, 5.0f, 60.0f });
	worldTrans_.scale_ = { 60.0f ,5.0f, 60.0f };
	SetCollisionAttribute(kCollisionAttributeWorld);
	SetCollisionMask(~kCollisionAttributeWorld);
}

void Ground::Update() {
	worldTrans_.UpdateMatrix();
	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));


}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTrans_, viewProjection);
}

void Ground::OnCollisionEnter() {
}

//void Ground::OnCollisionExit()
//{
//}

Vector3 Ground::GetWorldPosition() {
	Vector3 result{};

	result.x = worldTrans_.matWorld_.m[3][0];
	result.y = worldTrans_.matWorld_.m[3][1];
	result.z = worldTrans_.matWorld_.m[3][2];
	return result;
}
