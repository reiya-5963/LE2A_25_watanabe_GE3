#pragma once
#include "Collider.h"
#include <list>

/// <summary>
/// 当たり判定管理クラス
/// </summary>
class CollisionManager {
public: // メンバ関数
	/// <summary>
	/// 全ての当たり判定
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 判定にオブジェクトを追加
	/// </summary>
	/// <param name="collider"></param>
	void AddColliders(Collider* collider);

	/// <summary>
	/// コンテナの初期化
	/// </summary>
	void ClearColliders();

private: // メンバ関数
	/// <summary>
	/// 二つのオブジェクトの当たり判定
	/// </summary>
	/// <param name="colliderA">オブジェクトA</param>
	/// <param name="colliderB">オブジェクトB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private: // メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;
};

