#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) { 
	models_ = models;
	worldTrans_.Initialize();

}

void BaseCharacter::Update() { 
	worldTrans_.UpdateMatrix();

}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(worldTrans_, viewProjection);
	}
}