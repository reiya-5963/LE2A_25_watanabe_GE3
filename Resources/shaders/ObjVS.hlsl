#include "obj.hlsli"

VSOutput main(float32_t4 pos : POSITION, float32_t3 normal : NORMAL, float32_t2 uv : TEXCOORD)
{
	//
    float32_t4 worldNormal = normalize(mul(float32_t4(normal, 0), world));
    float32_t4 worldPos = mul(pos, world);
	
    VSOutput output;
    output.svPos = mul(pos, mul(world, mul(view, projection)));
    
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
	return output;
}