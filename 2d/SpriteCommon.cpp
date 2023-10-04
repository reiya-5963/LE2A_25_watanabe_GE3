#include "SpriteCommon.h"
#include "Sprite.h"
#include "MyMath.h"
#include <cassert>
#include "TextureManager.h"
#include "DXCManager.h"
#include "MyD3D12Create.h"

/// <summary>
/// 静的メンバ変数の実体生成
/// </summary>
ID3D12Device* SpriteCommon::sDevice_ = nullptr;
ID3D12GraphicsCommandList* SpriteCommon::sCommandList_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> SpriteCommon::sRootSignature_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> SpriteCommon::sPipelineState_ = nullptr;
Matrix4x4 SpriteCommon::sMatProjection_;


SpriteCommon* SpriteCommon::GetInstance() {
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::Initialize(int windowWidth, int windowHeight, const std::wstring& directoryPath) {
	assert(DirectXCommon::GetInstance()->GetDevice());
	sDevice_ = DirectXCommon::GetInstance()->GetDevice();

	//下準備
	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

#pragma region RootSignatureの生成

#pragma region デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRangeSRV[1]{};
	descriptorRangeSRV[0] = MyCreate::MyInit(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
#pragma endregion
#pragma region RootParameterの設定
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0] = MyCreate::MyInitAsConstantBufferView(0, D3D12_SHADER_VISIBILITY_ALL);
	rootParameters[1] = MyCreate::MyInitAsDescriptorTable(1, descriptorRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
#pragma endregion
#pragma region sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0] = MyCreate::MySampDescInit(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

#pragma endregion

	//RootSignatureの生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	descriptionRootSignature.pParameters = rootParameters;				//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);	//配列の長さ
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	result = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		MyLog::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に作成
	result = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region InputLayoutの設定
	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{	//xy座標
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	//InputLayoutの設定
	graphicsPipelineStateDesc.InputLayout.pInputElementDescs = inputElementDescs;
	graphicsPipelineStateDesc.InputLayout.NumElements = _countof(inputElementDescs);
#pragma endregion

#pragma region BlendStateの設定
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
#pragma endregion 

#pragma region Rasterrizerの設定
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
#pragma endregion

#pragma region 各シェーダーのコンパイル
	//各シェーダーの読み込みとコンパイル
	std::wstring vsFile = directoryPath + L"/shaders/SpriteVS.hlsl";
	vertexShaderBlob = DXCManager::GetInstance()->CompileShader(vsFile, L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
	std::wstring psFile = directoryPath + L"/shaders/SpritePS.hlsl";
	pixelShaderBlob = DXCManager::GetInstance()->CompileShader(psFile, L"ps_6_0");
	assert(pixelShaderBlob != nullptr);
#pragma endregion

#pragma region PSOの生成
	graphicsPipelineStateDesc.pRootSignature = sRootSignature_.Get();		//RootSignature
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };									//VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };										//PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;						//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;				//RsterizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むかの設定(気にしなくていい)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.DepthStencilState.DepthEnable = true;
	graphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	graphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//実際に生成
	result = sDevice_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&sPipelineState_));
	assert(SUCCEEDED(result));
#pragma endregion

	// 正射影行列の生成
	sMatProjection_ = MyMath::MakeOrthographicMatrix(0.0f, 0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 100.0f);

}

void SpriteCommon::PreDraw(ID3D12GraphicsCommandList* commandList) {

	//PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(sCommandList_ == nullptr);

	//コマンドリストをセット
	sCommandList_ = commandList;

	//ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sRootSignature_.Get());
	
	//パイプラインステートの設定
	sCommandList_->SetPipelineState(sPipelineState_.Get());
	
	//プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void SpriteCommon::PostDraw() {	
	//コマンドリストを解除
	sCommandList_ = nullptr;

}

Sprite* SpriteCommon::Create(uint32_t tectureHandle, Vector2 position, float rotate, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY) {
	// 仮サイズ
	Vector2 size = { 100.0f, 100.0f };

	{
		// テクスチャの情報を取得
		const D3D12_RESOURCE_DESC& resourceDesc =
			TextureManager::GetInstance()->GetResourceDesc(tectureHandle);
		// サイズをテクスチャのサイズに設定
		size = { (float)resourceDesc.Width, (float)resourceDesc.Height };
	}


	//Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(tectureHandle, position, rotate, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}


	//初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}
	return sprite;
}

