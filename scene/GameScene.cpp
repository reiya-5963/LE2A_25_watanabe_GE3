#include "GameScene.h"
#include <cassert>
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	delete test_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	
	uvCheckTex_ = TextureManager::Load("uvChecker.png");
	whiteBaseTex_ = TextureManager::Load("white1x1.png");

	cScale = { 1.0f, 1.0f, 1.0f };
	cRotate = { 0.0f, 0.0f, 0.0f };
	cTranslate = { 0.0f, 0.0f, -100.0f };

	// カメラ関係の初期化
	cameraMatrix = MyMath::MakeAffineMatrix(cScale, cRotate, cTranslate);
	viewMatrix = MyMath::Inverse(cameraMatrix);
	projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 100.0f);
	viewProjectionMatrix = MyMath::Multiply(viewMatrix, projectionMatrix);

	// Spriteの生成
	Sprite* sprite = Sprite::Create(uvCheckTex_, { 500.0f, 500.0f}, 0.0f);
	sprites_.push_back(sprite);

	Triangle* triangle_ = Triangle::Create(whiteBaseTex_, { 3.0f, 1.25f, 2.0f });
	Triangle* texTriangle_ = Triangle::Create(uvCheckTex_, { 3.0f, 1.25f, 2.0f });


	triangles_.push_back(triangle_);
	triangles_.push_back(texTriangle_);

	test_ = Model::CreateFlomObj("fence");
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

void GameScene::Update() {
	cameraMatrix = MyMath::MakeAffineMatrix(cScale, cRotate, cTranslate);
	viewMatrix = MyMath::Inverse(cameraMatrix);
	viewProjectionMatrix = MyMath::Multiply(viewMatrix, projectionMatrix);

	ImGui::DragFloat3("3dModelScale", &worldTransform_.scale_.x, 1.0f);
	ImGui::DragFloat3("3dModelRotate", &worldTransform_.rotation_.x, 1.0f);
	ImGui::DragFloat3("3dModelTranslation", &worldTransform_.translation_.x, 1.0f);

	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Create")) {
			if (ImGui::MenuItem("Sprite")) {

			}


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::DragFloat3("camera_rotate", &cRotate.x, 0.05f);
	ImGui::DragFloat3("camera_translate", &cTranslate.x, 0.05f);
	ImGui::NewLine();

	// スプライトに対して
	if (ImGui::CollapsingHeader("Sprite")) {
		// もしスプライトがなければ
		if (sprites_.size() == 0) {
			ImGui::Text("NoSprite...");
		}
		for (int i = 0; i < sprites_.size(); i++) {
			char sprName[32] = "";
			sprintf_s(sprName, "Sprite%d", i);

			float sRotate = sprites_[i]->GetRotate();
			Vector2 sTranslate = sprites_[i]->GetTranslate();
			Vector2 sSize = sprites_[i]->GetSize();
			Vector4 sColor = sprites_[i]->GetColor();
			Vector2 sTexSize = sprites_[i]->GetTexSize();
			Vector2 sTexBase = sprites_[i]->GetTexBase();

			if (ImGui::TreeNode(sprName)) {
				ImGui::DragFloat2("sprite_Translate", &sTranslate.x, 0.05f);
				ImGui::SliderAngle("sprate_Rotate_Z", &sRotate);
				ImGui::DragFloat2("sprite_size", &sSize.x, 0.05f, 0.0f);
				ImGui::ColorEdit4("sprite_color", &sColor.x);
				ImGui::DragFloat2("spriteTexture_size", &sTexSize.x, 0.05f, 0.0f);
				ImGui::DragFloat2("spriteTexture_base", &sTexBase.x, 0.05f, 0.0f);
				ImGui::TreePop();

			}

			sprites_[i]->SetRotate(sRotate);
			sprites_[i]->SetPosition(sTranslate);
			sprites_[i]->SetSize(sSize);
			sprites_[i]->SetColor(sColor);
			sprites_[i]->SetTextureRect(sTexBase, sTexSize);
		}
	}
	// 三角形に対して
	if (ImGui::CollapsingHeader("Triangle")) {
		if (triangles_.size() == 0) {
			ImGui::Text("NoTriangle...");
		}
		for (int i = 0; i < triangles_.size(); i++) {
			char triName[32] = "";
			sprintf_s(triName, "Triangle%d", i);

			// 三角形の座標取得
			Vector3 tScale = triangles_[i]->GetScale();
			Vector3 tRotate = triangles_[i]->GetRotate();
			Vector3 tTranslate = triangles_[i]->GetTranslate();
			Vector4 tColor = triangles_[i]->GetColor();

			if (ImGui::TreeNode(triName)) {
				ImGui::DragFloat3("triangle_translate", &tTranslate.x, 0.05f);
				ImGui::DragFloat3("triangle_rotate", &tRotate.x, 0.05f);
				ImGui::DragFloat3("triangle_scale", &tScale.x, 0.05f);
				ImGui::ColorEdit4("triangle_color", &tColor.x);
				ImGui::TreePop();
			}
			triangles_[i]->SetScale(tScale);
			triangles_[i]->SetRotate(tRotate);
			triangles_[i]->SetTranslate(tTranslate);
			triangles_[i]->SetColor(tColor);

		}

	}


	ImGui::End();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//SpriteCommon::PreDraw(commandList, SpriteCommon::BlendMode::kAdd);
	////この間に背景スプライトの描画を入れる
	//for (Sprite* sprite : sprites_) {
	//	sprite->Draw();
	//}
	//SpriteCommon::PostDraw();


	dxCommon_->ClearDepthBuffer();
	
	
	Model::PreDraw(commandList, Model::BlendMode::kAdd);

	test_->Draw(worldTransform_, viewProjection_);

	Model::PostDraw();

	Triangle::PreDraw(commandList);
	for (Triangle* triangle : triangles_) {
		triangle->Draw(viewProjectionMatrix);
	}
	Triangle::PostDraw();

	Sprite::PreDraw(commandList);
	
	//この間に背景スプライトの描画を入れる
	for (Sprite* sprite : sprites_) {
		sprite->Draw(Sprite::BlendMode::kAdd);
	}

	Sprite::PostDraw();

}