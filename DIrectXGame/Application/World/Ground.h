#pragma once
#include "Collider.h"
#include <cassert>

class Ground : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">位置</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	void OnCollisionEnter(int object);
	//void OnCollisionExit();

	Vector3 GetWorldPosition();
private:
	// ワールド変換データ
	//WorldTransform worldTrans_;
	// モデル
	Model* model_ = nullptr;
};