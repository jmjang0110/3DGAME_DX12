
#ifndef _VALUE
#define _VALUE

/*	

	[b] : Constant Buffer
	[t] : texture
	[c] : Buffer Offset 
	[s] : Sampler 
	[u] : Unordered Access View 


cbuffer
	- 상수 변수 사용에 최적화
	- CPU 가 빈번하게 갱신 
	- 쉐이더 상수를 그룹으로 묶어서 동시에 ( 한꺼번에 ) 갱신 
	  갱신의 빈도에 따라 상수를 그룹으로 묶는 것이 성능에 최적이다.
	- 하나의 상수버퍼는 4096개의 벡터 (4개의 32 비트 실수 ) 크기를 가질 수 있다 .
	- 파이프라인 마다 14개의 상수 버퍼를 연결 할 수 있다.
	- 파이프라인에 연결하기 위해 뷰가 필요 ( 쉐이더 변수 처럼 사용한다.

tbuffer 
	- 텍스처 버퍼 
	- 인덱스화된 데이터에 적합 
	- 텍스처 처럼 인덱스로 사용 
	- 파이프라인 마다 128개의 텍스처 버퍼를 연결(bind)할 수 있다.
	- 파이프 라인에 연결하기 위해 뷰가 필요하고 텍스처 슬롯에 연결 되어야 한다.


ConstantBuffer  - 사용자 정의 구조체의 상수 버퍼 
StructureBuffer - 구조체 ( 최대 2048 바이트 ) 의 버퍼 



*/


struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};

cbuffer cbCameraInfo : register(b0)
{
	matrix					gmtxView			: packoffset(c0);
	matrix					gmtxProjection		: packoffset(c4);
	float3					gvCameraPosition	: packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b1)
{
	matrix					gmtxGameObject		: packoffset(c0);
	MATERIAL				gMaterial			: packoffset(c4);
};

// [ b4 ] 는 light.hlsl 에 있음 

Texture2D gtxTexture		 : register(t0);
SamplerState gSamplerState	 : register(s0);


#define FRAME_BUFFER_WIDTH 800.0f
#define FRAME_BUFFER_HEIGHT 600.0f

#endif
