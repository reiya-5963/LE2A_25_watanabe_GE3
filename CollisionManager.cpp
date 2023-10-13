#include "CollisionManager.h"
#include <iterator>
void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider *colA = *itrA;

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
	colliders_.push_back(collider);
}

void CollisionManager::ClearColliders() {
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if (
		(colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask()) ||
		(colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask())) {
		return;
	}

	Vector3 positionA = colliderA->GetWorldPosition();
	Vector3 positionB = colliderB->GetWorldPosition();
	float radiusA = colliderA->GetRadius();
	float radiusB = colliderB->GetRadius();

	float distance =
		(positionA.x - positionB.x) * (positionA.x - positionB.x) +
		(positionA.y - positionB.y) * (positionA.y - positionB.y) +
		(positionA.z - positionB.z) * (positionA.z - positionB.z);

	if (distance <= (radiusA + radiusB) * (radiusA + radiusB)) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
