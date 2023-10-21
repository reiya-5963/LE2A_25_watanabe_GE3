#include "Skydome.h"
#include "R_Math.h"

/// <summary>
/// 
/// </summary>
/// <param name="model"></param>
/// <param name="position"></param>
void Skydome::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

/// <summary>
/// 
/// </summary>
void Skydome::Update() {
	worldTransform_.matWorld_ = R_Math::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
	//worldTransform_.UpdateMatrix();

}

/// <summary>
/// 
/// </summary>
/// <param name="viewProjection"></param>
void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
