#pragma once
#include "Collider.h"

class Wepon final : public Collider {
public:
	void Initialize(const std::vector<Model*>& models, WorldTransform *parent);

	void WorldTransformInitialize();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	const Vector3& GetRotation() const { return objectWorldTrans_.rotation_; }

	void SetRotation(const Vector3& rotation) { objectWorldTrans_.rotation_ = rotation; }

	void OnCollisionEnter(int object) override;

	Vector3 GetWorldPosition() override;

	void SetPlayerPos(Vector3 playerPos) { playerPos_ = playerPos; }

	void SetPlayerRotate(Vector3 playerRotate) { playerRotate_ = playerRotate; }
private:
	// モデルデータ配列
	std::vector<Model*> models_;
	
	Vector3 playerPos_;
	Vector3 playerRotate_;
};


