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

	void SetIsRespown(bool isRespown) {
		isRespown_ = isRespown;
	}

	/// <summary>
	/// ビュープロジェクションの取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	ViewProjection& GetViewProjection();

	void Reset();

	Vector3 CalcOffset() const;
private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	
	// fps視点でのカメラ位置
	Vector3 fpsView_ = { 0.0f, 4.0f, 0.0f };
	// tps視点でのカメラ位置
	Vector3 tpsView_ = { 0.0f, 4.0f, -30.0f };

	// ターゲットになるワールド変換データ
	const WorldTransform* target_ = nullptr;

	// マウスの位置
	POINT mousePos_;
	// 移動量を加算するためのやつ
	POINT preMousePos_;

	Vector3 offset_ = { 0.0f, 5.0f, 0.0f };

	Vector3 interTarget_ = {};


	bool isRespown_ = false;
	//
	bool isFps_ = false;
	float t_ = 0.1f;
	float destinationAngleY_ = 0.0f;
};