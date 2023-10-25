#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) { 
	models_ = models;
	objectWorldTrans_.Initialize();
	Collider::Initialize();

}

void BaseCharacter::Update() {
	UpdateWorldTransform();

	objectWorldTrans_.UpdateMatrix();

}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(objectWorldTrans_, viewProjection);
	}
}

Vector3 BaseCharacter::GetWorldPosition() {
	Vector3 result{};
	//
	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];

	return result;
}
