#pragma once
#include "DXCommon.h"
#include <dxcapi.h>
#include <memory>
#include <string>

#include "MyStruct.h"
#include "MyLog.h"

class Sprite;

class SpriteCommon {
public:
	static SpriteCommon* GetInstance();

	static void Initialize(
		int windowWidth, int windowHeight,
		const std::wstring& directoryPath = L"Resources/");

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="rotate">回転</param>
	/// <param name="scale">大きさ</param>
	/// <returns>生成されたスプライト</returns>
	static Sprite* Create(uint32_t tectureHandle, Vector2 position, float rotate,
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Vector2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false,
		bool isFlipY = false);

	static ID3D12GraphicsCommandList* GetsCommandList() { return sCommandList_; }
	
	static Matrix4x4 GetsMatProjection() { return sMatProjection_; }
private:
	SpriteCommon() = default;
	~SpriteCommon() = default;
	SpriteCommon(const SpriteCommon&) = delete;
	const SpriteCommon& operator = (const SpriteCommon&) = delete;

private:
	//デバイス
	static ID3D12Device* sDevice_;
	//コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	//ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	//パイプラインステート
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	//射影行列(消す予定)
	static Matrix4x4 sMatProjection_;

};