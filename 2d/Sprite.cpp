#include "Sprite.h"
#include <cassert>
#include "TextureManager.h"
#include "DXCManager.h"
#include "MyMath.h"
#include "MyStruct.h"
#include "MyD3D12Create.h"


/// <summary>
/// スプライト生成
/// </summary>
/// <param name="position">座標</param>
/// <param name="rotate">回転</param>
/// <param name="scale">大きさ</param>
/// <returns>生成されたスプライト</returns>
//Sprite* Sprite::Create(uint32_t tectureHandle, Vector2 position, float rotate,
//	Vector4 color, Vector2 anchorpoint, bool isFlipX,
//	bool isFlipY) {
//	// 仮サイズ
//	Vector2 size = { 100.0f, 100.0f };
//
//	{
//		// テクスチャの情報を取得
//		const D3D12_RESOURCE_DESC& resourceDesc =
//			TextureManager::GetInstance()->GetResourceDesc(tectureHandle);
//		// サイズをテクスチャのサイズに設定
//		size = { (float)resourceDesc.Width, (float)resourceDesc.Height };
//	}
//
//
//	//Spriteのインスタンスを生成
//	Sprite* sprite = new Sprite(tectureHandle, position, rotate, size, color, anchorpoint, isFlipX, isFlipY);
//	if (sprite == nullptr) {
//		return nullptr;
//	}
//
//
//	//初期化
//	if (!sprite->Initialize()) {
//		delete sprite;
//		assert(0);
//		return nullptr;
//	}
//	return sprite;
//}



/// <summary>
/// コンストラクタ
/// </summary>
Sprite::Sprite() {}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="rotate">回転</param>
/// <param name="scale">大きさ</param>
Sprite::Sprite(uint32_t textureHandle, Vector2 position, float rotate, Vector2 size,
	Vector4 color, Vector2 anchorPoint, bool isFlipX, bool isFlipY) {
	position_ = position;
	rotation_ = rotate;
	size_ = size;
	anchorPoint_ = anchorPoint;
	matWorld_ = MyMath::MakeIdentity4x4();
	color_ = color;
	textureHandle_ = textureHandle;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	texSize_ = size;
}

/// <summary>
/// 初期化
/// </summary>
/// <returns>できたかどうか</returns>
bool Sprite::Initialize() {
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	HRESULT result = S_FALSE;

	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	{//vertexResourceの生成
		D3D12_HEAP_PROPERTIES heapProps = MyCreate::SetHeapPro(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = MyCreate::SetBufferResourceDesc(sizeof(VertexPosUv) * kVertexNum);


		result = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&vertexBuffer_));
		assert(SUCCEEDED(result));

		//vertexBuffer_ = CreateBufferResource(sizeof(Vector4) * kVertexNum);
		result = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexMap_));
		assert(SUCCEEDED(result));
	}
	//頂点バッファへの転送
	TransferVertices();

	//頂点バッファビューの生成
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexPosUv) * kVertexNum;
	vertexBufferView_.StrideInBytes = sizeof(VertexPosUv);

	{//定数バッファの生成
		D3D12_HEAP_PROPERTIES heapProps = MyCreate::SetHeapPro(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = MyCreate::SetBufferResourceDesc((sizeof(ConstBufferData) + 0xff) & ~0xff);


		result = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffer_));
		assert(SUCCEEDED(result));


		//constBuffer_ = CreateBufferResource(sizeof(ConstBufferData));
		result = constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constMap_));
		assert(SUCCEEDED(result));
	}

	return true;
}


/// <summary>
/// 描画
/// </summary>
/// <param name="viewProMat">ViewProjectionMatrix</param>
void Sprite::Draw() {
	//ワールド行列の計算
	//Matrix4x4 TransWorld = MyMath::MakeAffineMatrix(worldTrans_.scale, worldTrans_.rotate, worldTrans_.translate);
	matWorld_ = MyMath::MakeIdentity4x4();
	matWorld_ = MyMath::Multiply(matWorld_, MyMath::MakeRotateZMatrix(rotation_));
	matWorld_ = MyMath::Multiply(matWorld_, MyMath::MakeTranslateMatrix({ position_.x, position_.y, 0.0f }));


	//ワールド行列とビュープロジェクション行列の合成
	//matWorld_ = MyMath::Multiply(matWorld_, viewProMat);


	//それぞれ定数バッファに代入
	constMap_->color = color_;
	constMap_->mat = MyMath::Multiply(matWorld_, SpriteCommon::GetsMatProjection());

	//頂点バッファの設定
	SpriteCommon::GetsCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//定数バッファビューをセット
	SpriteCommon::GetsCommandList()->SetGraphicsRootConstantBufferView(0, constBuffer_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(SpriteCommon::GetsCommandList(), 1, textureHandle_);
	//描画コマンド
	SpriteCommon::GetsCommandList()->DrawInstanced(kVertexNum, 1, 0, 0);
}

/// <summary>
/// 頂点データ転送
/// </summary>
void Sprite::TransferVertices() {
	//HRESULT result = S_FALSE;

		//左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}


	VertexPosUv vertices[kVertexNum];

	vertices[LB].pos = { left, bottom, 0.0f };
	vertices[LT].pos = { left, top, 0.0f };
	vertices[RB].pos = { right, bottom, 0.0f };
	vertices[RT].pos = { right, top, 0.0f };

	{
		float texLeft = texBase_.x / resourceDesc_.Width;
		float texRight = (texBase_.x + texSize_.x) / resourceDesc_.Width;
		float texTop = texBase_.y / resourceDesc_.Height;
		float texBottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;

		vertices[LB].uv = { texLeft, texBottom };
		vertices[LT].uv = { texLeft, texTop };
		vertices[RB].uv = { texRight, texBottom };
		vertices[RT].uv = { texRight, texTop };
	}



	//バッファに転送
	memcpy(vertexMap_, vertices, sizeof(vertices));
}

void Sprite::SetSize(const Vector2& size) {
	size_ = size;
	TransferVertices();
}

void Sprite::SetPosition(const Vector2& position) {
	position_ = position;
	TransferVertices();
}

void Sprite::SetRotate(const float& rotate) {
	rotation_ = rotate;
	TransferVertices();
}

void Sprite::SetColor(const Vector4& color) {
	color_ = color;
}

void Sprite::SetTextureRect(const Vector2& texBase, const Vector2 texSize) {
	texBase_ = texBase;
	texSize_ = texSize;
	TransferVertices();

}

void Sprite::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
}