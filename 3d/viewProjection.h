#pragma once

#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>

/// <summary>
/// 
/// </summary>
struct ConstBufferDataViewProjection {
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector3 cameraPos;
};

struct ViewProjection {
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	ConstBufferDataViewProjection* constMap = nullptr;

#pragma region ビュー行列の設定
	// 
	Vector3 translate = { 0.0f, 0.0f, -100.0f };
	// 
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	// 
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = 0.45f;
	// ビューポートのアスペクト比
	float aspectRatio = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);
	// 深度限界(手前側)
	float nearZ = 0.1f;
	// 深度限界(奥側)
	float farZ = 100.0f;
#pragma endregion
	// ビュー行列
	Matrix4x4 matView;
	// 射影行列
	Matrix4x4 matProjection;

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
	void UpdateMatrix();

	/// <summary>
	/// 
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// 
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 
	/// </summary>
	void UpdateProjectionMatrix();
};