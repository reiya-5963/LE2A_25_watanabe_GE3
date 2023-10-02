//#pragma once
//#include "DXCommon.h"
//#include "Input.h"
//
//#include "ImGuiManager.h"
//#include "TextureManager.h"
//#include "DXCManager.h"
//#include "WinApp.h"
//
//class Framework {
//public:
//	
//	virtual void ~Framework() = default;
//
//	virtual void Initialize();
//
//	virtual void Finalize();
//
//	virtual void Update();
//
//	virtual void Draw() = 0;
//
//	virtual bool IsEndRequest() { return endRequest_; }
//	
//	// 実行関数
//	void Run();
//protected:	
//	WinApp* winApp_;
//	DirectXCommon* dxCommon_;
//	Input* input_;
//	ImGuiManager* imGuiManager_;
//
//	bool endRequest_ = false;
//};