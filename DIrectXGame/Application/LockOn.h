#pragma once
#include <memory>
#include "Sprite.h"
#include <list>
#include "viewProjection.h"
#include "Enemy.h"
//ロックオン
class LockOn {
public:
	void Initialize();

	void Update(const std::list< std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewPro);

	void Draw();

	void SearchTarget(const std::list< std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewPro);

	bool Judge2RangeOut(const ViewProjection& viewPro);
 
	Vector3 World2ScreenPos(Vector3 world, const ViewProjection& viewPro);

	bool ExistTarget() const { 
		return target_ ? true : false;
	}

	Vector3 GetTargetPosition() const;
private:
	std::unique_ptr<Sprite> lockOnMark_;
	const Enemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 100.0f;
	// 角度範囲
	float angleRange_ = 20.0f;
};

