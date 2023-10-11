#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WinApp.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// ビュープロジェクションの取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	ViewProjection& GetViewProjection();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	
	// 追従対象
	const WorldTransform* target_ = nullptr;

	POINT mousePos_;
	POINT preMousePos_;


};