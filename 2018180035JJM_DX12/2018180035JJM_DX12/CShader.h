#pragma once

class CShader
{

private:
	SHADER_TYPE							m_eType{};

	ComPtr<ID3DBlob>					m_VSBlob{};
	ComPtr<ID3DBlob>					m_PSBlob{};

	ComPtr<ID3DBlob>					m_ErrorBlob{};
	D3D12_SHADER_BYTECODE				m_ShaderByteCode{};

public:
	CShader();
	~CShader();
public:
	BOOL OnCreate(SHADER_TYPE		   eShaderType,
				   const LPCWSTR& wstrFileName,
				   const LPCSTR&  strFuncName,
				   const LPCSTR&  strShaderVersion);

	ComPtr<ID3DBlob> GetBlob(SHADER_TYPE eShaderType);
	SHADER_TYPE GetShaderType() { return m_eType; }
	D3D12_SHADER_BYTECODE GetShaderByteCode() { return m_ShaderByteCode; }



public:
	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables();
	virtual void ReleaseShaderVariables();


};


/// Shader Resource 는 SRV, CBV, UAV 가 있다. 
///		- 이 세개가 리소스를 만드록 파이프라인에 연결한다.
///		- 이런 리소스를 쉐이더에 글로벌 변수들을 선언해서 사용한다.
///		- 데이터 타입 , 변수 이름, 콜론 하고 레지스터 키워드를 붙이고
///		- shader profile 생략가능
///
///		Constant Buffer 
///		c0[float4]
///		c1[float2][float2] - 변수 선언 float2 g1, float2 g2  
///	b0	c2[...]
///		c3[...]
///		c4[...]
/// 
/// 어떤 레지스터에 변수를 만들것인가?? 어떤 레지스터 타입에 어떤 주소공간에 만들것인가.
/// 
/// b0 레지스터 
/// register (b0 ) - packoffset(c0)
/// 
/// packoffset : 상수버퍼에 대해서만 적용한다. 
///				 주의할 점 1. 하나의 변수는 c0 c1 레지스터 공간에 걸쳐져 있을 수 없다.
///							  float4 라는 벡터 공간에서 온전하게 변수들의 주소들이 지정이 되도록 만들어져야한다. 
///							그래서 packoffset 을 쓰던 쓰지 않던 변수들을 지정하는데
/// 하나의 변수가 두개의 레지스터 메모리에 걸치게 되면 자동으로 넘어간다. 그래서
/// cbuffer 
/// 
/// Cbuffer { 무조건 비어있지 않게 만들어야한다. 
///		float2 c0-[0][0][x][x] - 건너 뛰겠지... 근데 이상하게 읽게 된다. 그래서 메모리가 비면 안된다.! 순서 또는 비면 문제가 생긴다. 
///		float4 c1-[0][0][0][0]					-> 안쓰면 dummy 를 써야한다. 
///		float2 c2-[0][0][x][x]
///		
/// } --> 중간에 메모리가 비는 부분이 있다. 그래서 유의해야한다!!!! 
/// 
/// pack offset 을 통해서 세부적인 지정을 할 수 있다. 
/// 
/// 
///  256 바이트 메모리로만 상수 버퍼를 만들 수 있다. 
///  벡터 형태로만 선언할 수 있다. 