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

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static SpriteCommon* GetInstance();
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="windowWidth">ウィンドウの横幅</param>
	/// <param name="windowHeight">ウィンドウの縦幅</param>
	/// <param name="directoryPath">リソースファイルのパス</param>
	static void Initialize(
		int windowWidth, int windowHeight,
		const std::wstring& directoryPath = L"Resources/");

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
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

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	static ID3D12GraphicsCommandList* GetsCommandList() { return sCommandList_; }

	/// <summary>
	/// 正射影行列の取得
	/// </summary>
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