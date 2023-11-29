#pragma once
#include "Collider.h"
#include <cassert>

class Scaffold : public Collider {
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

	void SetIsMove(bool isMove);
	void OnCollisionEnter(int object);
	void OnCollisionExit();
	Vector3  GetWorldPosition() const;
private:
	// ワールド変換データ
	//WorldTransform worldTrans_;
	// モデル
	Model* model_ = nullptr;

	bool isMoveLeft_ = false;
	bool isMove_ = false;
};