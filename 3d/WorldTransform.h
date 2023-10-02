#pragma once

#include "MyStruct.h"
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>


struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld;
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// スケール
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	// 回転角
	Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };
	// 座標
	Vector3 translation_ = { 0.0f, 0.0f, 0.0f };
	// 行列
	Matrix4x4 matWorld_;
	// 親となるポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 
	/// </summary>
	void Initialize();

	/// <summary>
	/// 
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// 
	/// </summary>
	void Map();

	/// <summary>
	/// 
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// 
	/// </summary>
	void UpdateMatrix();
};