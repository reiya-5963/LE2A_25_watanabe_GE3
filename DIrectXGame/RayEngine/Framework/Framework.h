#pragma once
#include "DXCommon.h"
#include "Input.h"

#include "ImGuiManager.h"
#include "TextureManager.h"
#include "DXCManager.h"
#include "WinApp.h"

#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"

#include "SceneManager.h"
class Framework {
public:
	
	virtual ~Framework() = default;

	virtual void Initialize();

	virtual void Finalize();

	void PreUpdate();

	void PostUpdate();

	virtual void Update();

	virtual void Draw() = 0;

	virtual bool IsEndRequest() { return endRequest_; }
	
	// 実行関数
	void Run();
protected:	
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	ImGuiManager* imGuiManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;

	bool endRequest_ = false;
	//AbstractSceneFactory* sceneFactory_ = nullptr;
};