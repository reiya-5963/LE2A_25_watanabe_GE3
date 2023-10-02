#pragma once
#include "DXCommon.h"
#include "GameScene.h"

#include "ImGuiManager.h"
#include "TextureManager.h"
#include "DXCManager.h"
#include "WinApp.h"

class MyGame {
public:
	//
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	bool IsEndRequest() { return endRequest_; }
private:
	WinApp* winApp_;
	DirectXCommon* dxCommon_;
	Input* input_;
	ImGuiManager* imGuiManager_;
	
	bool endRequest_ = false;
	GameScene* gameScene_;
};