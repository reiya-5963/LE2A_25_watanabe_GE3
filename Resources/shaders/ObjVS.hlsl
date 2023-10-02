#include "obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//
    float4 worldNormal = normalize(mul(float4(normal, 0), world));
    float4 worldPos = mul(pos, world);
	
    VSOutput output;
    output.svPos = mul(pos, mul(world, mul(view, projection)));
    
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
	return output;
}