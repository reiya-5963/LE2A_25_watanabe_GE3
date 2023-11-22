cbuffer WorldTransform : register(b0)
{
    matrix world; // ワールド行列
};

cbuffer ViewProjection : register(b1)
{
    matrix view;        // ビュー変換行列
    matrix projection;  // プロジェクション行列
    float32_t3 cameraPos; // カメラ座標 (ワールド座標)
};

cbuffer Material : register(b2)
{
    float32_t3 m_ambient : packoffset(c0); // アンビエント係数
    float32_t3 m_diffuse : packoffset(c1);  // ディフューズ係数
    float32_t3 m_specular : packoffset(c2); // スペキュラー係数
    float32_t1 m_alpha : packoffset(c2.w);   // アルファ
}

// 平行光源の数
static const int DIRECTLIGHT_NUM = 3;

struct DirectLight
{
    float32_t3 lightv; // ライトへの方向の単位ベクトル
    float32_t3 lightcolor; // ライトの色(rgb)
    uint active;        // 有効かどうか
};

static const int POINTLIGHT_NUM = 3;

struct PointLight
{
    float32_t3 lightPos; // ライト座標
    float32_t3 lightColor; // ライトの色(rgb)
    float32_t3 lightAtten; // ライト距離減衰係数
    uint active;        // 有効かどうか
};

static const int SPOTLIGHT_NUM = 3;

//
struct SpotLight
{
    float32_t3 lightv; // ライトの光線方向の逆ベクトル (単位ベクトル)
    float32_t3 lightPos; // ライト座標
    float32_t3 lightColor; // ライトの色(rgb)
    float32_t3 lightAtten; // ライト距離減衰係数
    float32_t2 lightFactorAngleCos; // ライト減衰角度のコサイン
    uint active;                // 有効かどうか
};

// 丸影の数
static const int CIRCLESHADOW_NUM = 3;

struct CircleShadow
{
    float32_t3 direction; // 投影方向の逆ベクトル (単位ベクトル)
    float32_t3 casterPos; // キャスター
    float32_t1 distanceCasterLight; // キャスターとライトの距離
    float32_t3 atten; // 距離減衰係数
    float32_t2 factorAngleCos; // 減衰角度のコサイン
    uint active;                // 有効かどうか
};


cbuffer LightGroup : register(b3)
{
    float32_t3 ambientColor;
    DirectLight directLight[DIRECTLIGHT_NUM];
    //PointLight pointLight[POINTLIGHT_NUM];
   // SpotLight spotLight[SPOTLIGHT_NUM];
   // CircleShadow circleShadow[CIRCLESHADOW_NUM];
}

struct VSOutput
{
    float32_t4 svPos : SV_POSITION;
    float32_t4 worldPos : POSITION;
    float32_t3 normal : NORMAL;
    float32_t2 uv : TEXCOORD;
};