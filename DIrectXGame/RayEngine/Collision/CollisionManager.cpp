#include "CollisionManager.h"
#include <iterator>

#include "R_Math.h"

void CollisionManager::Initialize() {
	model_.reset(Model::CreateFlomObj("BaseElipse"));

}

void CollisionManager::UpdateWorldTransform() {
	for (Collider* collider : colliders_) {
		collider->UpdateWorldTransform();
	}
}

void CollisionManager::Draw(const ViewProjection& viewProjection) {
	for (Collider* collider : colliders_) {
		collider->Draw(model_.get(), viewProjection);
	}
}

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colA = *itrA;

		// イテレータBはイテレータAの次の要素から回す (重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colA, colB);
		}
	}
}

void CollisionManager::AddColliders(Collider* collider) {
	// コンテナに追加
	colliders_.push_back(collider);
}

void CollisionManager::ClearColliders() {
	// コンテナを初期化
	if (colliders_.size() != 0) {
		colliders_.clear();
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	colliderA->SetParent(nullptr);
	colliderB->SetParent(nullptr);

	// 衝突フィルタリング (もし同じ属性なら判定しない)
	if (
		(colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask()) ||
		(colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask())) {
		return;
	}

	//// AとBの位置を取得
	//Vector3 positionA = colliderA->GetWorldPosition();
	//Vector3 positionB = colliderB->GetWorldPosition();
	//// AとBの半径取得
	//float radiusA = colliderA->GetRadius();
	//float radiusB = colliderB->GetRadius();

	//// 距離を計算
	//float distance =
	//	(positionA.x - positionB.x) * (positionA.x - positionB.x) +
	//	(positionA.y - positionB.y) * (positionA.y - positionB.y) +
	//	(positionA.z - positionB.z) * (positionA.z - positionB.z);

	// もし当たってたら
	//if (distance <= (radiusA + radiusB) * (radiusA + radiusB)) {
	if (IsCollision(colliderA, colliderB)) {
		colliderA->OnCollision();
		colliderA->SetParent(&colliderB->GetWorldTransform());
		colliderB->OnCollision();
		colliderB->SetParent(&colliderA->GetWorldTransform());
	}

}

bool CollisionManager::IsCollision(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetMin().x <= colliderB->GetMax().x && colliderA->GetMax().x >= colliderB->GetMin().x) &&
		(colliderA->GetMin().y <= colliderB->GetMax().y && colliderA->GetMax().y >= colliderB->GetMin().y) &&
		(colliderA->GetMin().z <= colliderB->GetMax().z && colliderA->GetMax().z >= colliderB->GetMin().z)) {

		return true;
	}

	return false;
}
