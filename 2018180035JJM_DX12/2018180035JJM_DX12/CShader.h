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


/// Shader Resource �� SRV, CBV, UAV �� �ִ�. 
///		- �� ������ ���ҽ��� ����� ���������ο� �����Ѵ�.
///		- �̷� ���ҽ��� ���̴��� �۷ι� �������� �����ؼ� ����Ѵ�.
///		- ������ Ÿ�� , ���� �̸�, �ݷ� �ϰ� �������� Ű���带 ���̰�
///		- shader profile ��������
///
///		Constant Buffer 
///		c0[float4]
///		c1[float2][float2] - ���� ���� float2 g1, float2 g2  
///	b0	c2[...]
///		c3[...]
///		c4[...]
/// 
/// � �������Ϳ� ������ ������ΰ�?? � �������� Ÿ�Կ� � �ּҰ����� ������ΰ�.
/// 
/// b0 �������� 
/// register (b0 ) - packoffset(c0)
/// 
/// packoffset : ������ۿ� ���ؼ��� �����Ѵ�. 
///				 ������ �� 1. �ϳ��� ������ c0 c1 �������� ������ ������ ���� �� ����.
///							  float4 ��� ���� �������� �����ϰ� �������� �ּҵ��� ������ �ǵ��� ����������Ѵ�. 
///							�׷��� packoffset �� ���� ���� �ʴ� �������� �����ϴµ�
/// �ϳ��� ������ �ΰ��� �������� �޸𸮿� ��ġ�� �Ǹ� �ڵ����� �Ѿ��. �׷���
/// cbuffer 
/// 
/// Cbuffer { ������ ������� �ʰ� �������Ѵ�. 
///		float2 c0-[0][0][x][x] - �ǳ� �ٰ���... �ٵ� �̻��ϰ� �а� �ȴ�. �׷��� �޸𸮰� ��� �ȵȴ�.! ���� �Ǵ� ��� ������ �����. 
///		float4 c1-[0][0][0][0]					-> �Ⱦ��� dummy �� ����Ѵ�. 
///		float2 c2-[0][0][x][x]
///		
/// } --> �߰��� �޸𸮰� ��� �κ��� �ִ�. �׷��� �����ؾ��Ѵ�!!!! 
/// 
/// pack offset �� ���ؼ� �������� ������ �� �� �ִ�. 
/// 
/// 
///  256 ����Ʈ �޸𸮷θ� ��� ���۸� ���� �� �ִ�. 
///  ���� ���·θ� ������ �� �ִ�. 