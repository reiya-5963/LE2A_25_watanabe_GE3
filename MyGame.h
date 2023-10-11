#pragma once
#include "DXCommon.h"
#include "GameScene.h"
#include "GlobalVariables.h"

#include "ImGuiManager.h"
#include "TextureManager.h"
#include "DXCManager.h"
#include "WinApp.h"

/// <summary>
/// ゲームクラス
/// </summary>
class MyGame {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲーム終了リクエスト(真: ループを抜ける 偽: 継続)
	/// </summary>
	/// <returns>終了リクエスト</returns>
	bool IsEndRequest() { return endRequest_; }
private:
	// windowAPI
	WinApp* winApp_;
	
	// DirectX汎用クラス
	DirectXCommon* dxCommon_;
	
	// 入力
	Input* input_;
	
	// ImGui管理クラス
	ImGuiManager* imGuiManager_;
	
	// ゲーム終了リクエストフラグ
	bool endRequest_ = false;

	// シーン
	GameScene* gameScene_;
};