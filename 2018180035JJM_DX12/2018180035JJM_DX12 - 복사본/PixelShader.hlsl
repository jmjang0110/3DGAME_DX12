#include "value.hlsl"
#include "Light.hlsl"
#include "VertexShader.hlsl"

// ============================== Deafult =================================
float4 PS_Main(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f; // (0, 1) : (-0.5, 0.5)
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    
    float fRadius = 0.3f;
    float fRadian = radians(360.0f / 30.0f);
    for (float f = 0; f < 30.0f; f += 1.0f)
    {
        float fAngle = fRadian * f;
        cColor.rgb += (0.0025f / length(f2NDC + float2(fRadius * cos(fAngle), fRadius * sin(fAngle))));
    }
    
    
    return (cColor);
}


// ============================= Light ======================================
float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
#ifdef _WITH_VERTEX_LIGHTING
	return(input.color);
#else
    input.normalW = normalize(input.normalW);
    float4 color = Lighting(input.positionW, input.normalW);

    return (color);
#endif
}

// ============================= Basic Mesh No Light ======================================
float4 PS_Basic(VS_BASIC_OUTPUT input) : SV_TARGET
{
    
    //return input.color;
    float4 cColor = gtxTexture.Sample(gSamplerState, input.uv);

    return (cColor);
}
