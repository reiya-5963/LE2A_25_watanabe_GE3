#include "Ground.h"

void Ground::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	objectWorldTrans_.Initialize();
	objectWorldTrans_.translation_ = position;
	objectName_ = int(ObjName::WORLD);

	Collider::Initialize();
	SetRadius({ 60.0f, 5.0f, 60.0f });
	objectWorldTrans_.scale_ = { 60.0f ,5.0f, 60.0f };
	SetCollisionAttribute(kCollisionAttributeWorld);
	SetCollisionMask(~kCollisionAttributeWorld);
}

void Ground::Update() {
	objectWorldTrans_.UpdateMatrix();
	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));


}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(objectWorldTrans_, viewProjection);
}

void Ground::OnCollisionEnter(int) {
}

//void Ground::OnCollisionExit()
//{
//}

Vector3  Ground::GetWorldPosition()const {
	Vector3 result{};

	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];
	return result;
}
