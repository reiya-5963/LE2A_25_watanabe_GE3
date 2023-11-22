#include "Obj.hlsli"

Texture2D<float32_t4> tex : register(t0);   // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

float32_t4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float32_t4 texcolor = tex.Sample(smp, input.uv);
    
	// 光沢度
    const float shininess = 4.0f;
	
    // 頂点殻視点への方向ベクトル (単位ベクトル)
    float32_t3 eyedir = normalize(cameraPos - input.worldPos.xyz);
	
    // 環境反射光
    float32_t3 ambient = m_ambient;

    // シェーディングによる色	
    float32_t4 shadecolor = float32_t4(ambientColor * ambient, m_alpha);
	
    //// もしアルファ値が透明に近ければ 後の処理を飛ばす
    //if (texcolor.a <= 0.5)
    //{
    //    discard;
    //}

    // 平行光源
    for (int i = 0; i < DIRECTLIGHT_NUM; i++)
    {
        if (directLight[i].active)
        {
            // ライトに向かうベクトルと法線の内積
            float32_t3 dotLightNormal = dot(directLight[i].lightv, normalize(input.normal));
            
            // 反射光ベクトル
            float32_t3 reflect = normalize(-directLight[i].lightv + 2 * dotLightNormal * input.normal);
            
            // 拡散反射光
            float32_t3 diffuse = dotLightNormal * m_diffuse;
            
            // 鏡面反射光
            float32_t3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            // 全て加算する
            shadecolor.rgb += (diffuse + specular) * directLight[i].lightcolor;
        }
    }
    
	// 点光源
    //for (i = 0; i < POINTLIGHT_NUM; i++)
    //{
    //    if (pointLight[i].active)
    //    {
    //        // ライトへの方向ベクトル
    //        float3 lightv = pointLight[i].lightPos - input.worldPos.xyz;
    //        float d = length(lightv);
    //        lightv = normalize(lightv);
            
            
    //        // 距離減衰係数
    //        float atten = 1.0f / (pointLight[i].lightAtten.x + pointLight[i].lightAtten.y * d + pointLight[i].lightAtten.z * d * d);
            
            
    //        // ライトに向かうベクトルと法線の内積
    //        float3 dotLightNormal = dot(lightv, input.normal);
           
    //        // 反射光ベクトル
    //        float3 reflect = normalize(-lightv + 2 * dotLightNormal * input.normal);
            
    //        // 拡散反射光
    //        float3 diffuse = dotLightNormal * m_diffuse;
           
    //        // 鏡面反射光
    //        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
    //        // 全て加算する
    //        shadecolor.rgb += atten * (diffuse + specular) * pointLight[i].lightColor;
    //    }
    //}
    
    
    float32_t4 result = shadecolor * texcolor;
    result.a = 0.0f;
    return result;
}