
#ifndef _VALUE
#define _VALUE

/*	

	[b] : Constant Buffer
	[t] : texture
	[c] : Buffer Offset 
	[s] : Sampler 
	[u] : Unordered Access View 


cbuffer
	- ��� ���� ��뿡 ����ȭ
	- CPU �� ����ϰ� ���� 
	- ���̴� ����� �׷����� ��� ���ÿ� ( �Ѳ����� ) ���� 
	  ������ �󵵿� ���� ����� �׷����� ���� ���� ���ɿ� �����̴�.
	- �ϳ��� ������۴� 4096���� ���� (4���� 32 ��Ʈ �Ǽ� ) ũ�⸦ ���� �� �ִ� .
	- ���������� ���� 14���� ��� ���۸� ���� �� �� �ִ�.
	- ���������ο� �����ϱ� ���� �䰡 �ʿ� ( ���̴� ���� ó�� ����Ѵ�.

tbuffer 
	- �ؽ�ó ���� 
	- �ε���ȭ�� �����Ϳ� ���� 
	- �ؽ�ó ó�� �ε����� ��� 
	- ���������� ���� 128���� �ؽ�ó ���۸� ����(bind)�� �� �ִ�.
	- ������ ���ο� �����ϱ� ���� �䰡 �ʿ��ϰ� �ؽ�ó ���Կ� ���� �Ǿ�� �Ѵ�.


ConstantBuffer  - ����� ���� ����ü�� ��� ���� 
StructureBuffer - ����ü ( �ִ� 2048 ����Ʈ ) �� ���� 



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

// [ b4 ] �� light.hlsl �� ���� 

Texture2D gtxTexture		 : register(t0);
SamplerState gSamplerState	 : register(s0);


#define FRAME_BUFFER_WIDTH 800.0f
#define FRAME_BUFFER_HEIGHT 600.0f

#endif
