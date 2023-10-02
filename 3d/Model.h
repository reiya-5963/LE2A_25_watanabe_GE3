#pragma once
#include "TextureManager.h"
#include "MyStruct.h"
#include "Mesh.h"
#include "LightGroup.h"
#include "WorldTransform.h"
#include "viewProjection.h"
#include <string>
#include <unordered_map>
#include <vector>

/// <summary>
/// モデルデータ
/// </summary>
class Model {
public: // 列挙子
	enum class RootParameter {
		kWorldTransform,	// ワールド変換行列
		kViewProjection,	// ビュープロジェクション変換行列
		kMaterial,			// マテリアル
		kTexture,			// テクスチャ
		kLight,				// ライト
	};

private: 
	static const std::string kBaseDirectory;
	static const std::string kDefaultModelName;

private: // 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescreptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup_;
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// グラフィックスパイプラインの初期化
	/// </summary>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static Model* Create();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	static Model* CreateFlomObj(const std::string& modelname);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="commandList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 
	/// </summary>
	static void PostDraw();
public:

	/// <summary>
	/// 
	/// </summary>
	~Model();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smoothing"></param>
	void Initialize(const std::string& modelname);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	void Draw(
		const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	/// <param name="textireHandle"></param>
	void Draw(
		const WorldTransform& worldTransform, const ViewProjection& viewProjection,
		uint32_t textireHandle);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	inline const std::vector<Mesh*>& GetMeshes() { return meshes_; }

private:
	// 名前
	std::string name_;
	// メッシュコンテナ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;
private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smoothing"></param>
	void LoadModel(const std::string& modelname);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="material"></param>
	void AddMaterial(Material* material);

	/// <summary>
	/// 
	/// </summary>
	void LoadTextures();
};

