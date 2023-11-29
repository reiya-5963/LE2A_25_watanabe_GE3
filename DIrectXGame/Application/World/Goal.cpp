#include "Goal.h"

void Goal::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	objectWorldTrans_.Initialize();
	objectWorldTrans_.translation_ = position;
	objectName_ = int(ObjName::ENEMY);

	Collider::Initialize();
	SetRadius({ 3.0f, 3.0f, 3.0f });
	objectWorldTrans_.scale_ = { 3.0f ,3.0f, 3.0f };
	SetCollisionAttribute(kCollisionAttributeWorld);
	SetCollisionMask(~kCollisionAttributeWorld);

}

void Goal::Update() {
	objectWorldTrans_.UpdateMatrix();
	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));

}

void Goal::Draw(ViewProjection& viewProjection) {
	model_->Draw(objectWorldTrans_, viewProjection);

}

void Goal::OnCollisionEnter(int)
{
}

Vector3  Goal::GetWorldPosition()const {
	Vector3 result{};

	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];
	return result;

}
