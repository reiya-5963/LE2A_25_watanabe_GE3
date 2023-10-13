#pragma once
#include "MyStruct.h"
#include <cstdint>
#include "CollisionConfig.h"
/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
private:
	// 衝突属性 (自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク (相手)
	uint32_t CollisionMask_ = 0xffffffff;

public: // メンバ関数
	// 半径の取得
	float GetRadius() { return radius_; }
	
	// 半径の設定
	void SetRadius(const float radius);

	// 衝突属性 (自分)
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(uint32_t collisionAttribute);

	// 衝突マスク (相手)
	uint32_t GetCollisionMask();
	void SetCollisionMask(uint32_t collisionMask);

	// 衝突時に呼ばれる関数
	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;
private: // メンバ変数　
	// 衝突半径
	float radius_ = 1.0f;
};

