
#include "value.hlsl"
// 스위즐링 : 순서를 바꿔 쓴다. 
//프리미티브(삼각형)를 구성하는 정점의 인덱스(SV_VertexID)에 따라 정점을 반환한다. 
//정점의 위치 좌표는 변환이 된 좌표(SV_POSITION)이다. 즉, 투영좌표계의 좌표이다. 

// ============================== Deafult =================================
//정점 셰이더를 정의한다.
float4 VS_Main(uint nVertexID : SV_VertexID) : SV_POSITION
{
    float4 output = (float4) 0;

    if (nVertexID == 0)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 1)
        output = float4(+1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 2)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 3)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 4)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 5)
        output = float4(-1.0f, -1.0f, 0.0f, 1.0f);
    return (output);
    
}


// ============================= Light ======================================
struct VS_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR;
#endif
};

VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
    VS_LIGHTING_OUTPUT output;

    output.normalW = mul(input.normal, (float3x3) gmtxGameObject);
    output.positionW = (float3) mul(float4(input.position, 1.0f), gmtxGameObject);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
    return (output);
}



// ============================= Basic Mesh No Light ======================================
struct VS_BASIC_INPUT
{
    float3 position : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};

struct VS_BASIC_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};
    
VS_BASIC_OUTPUT VS_Basic(VS_BASIC_INPUT input)
{
    VS_BASIC_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.f), gmtxGameObject), gmtxView), gmtxProjection);
    output.color    = input.color;
    output.uv       = input.uv;
    
    return output;
}



