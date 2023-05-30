#pragma once

class CShader
{

private:
	SHADER_TYPE							m_eType{};

	ComPtr<ID3DBlob>					m_VSBlob{};
	ComPtr<ID3DBlob>					m_PSBlob{};

public:
	CShader();
	~CShader();
public:
	BOOL OnCreate(SHADER_TYPE		   eShaderType,
				   const LPCWSTR& wstrFileName,
				   const LPCSTR&  strFuncName,
				   const LPCSTR&  strShaderVersion);

	ComPtr<ID3DBlob> GetBlob(SHADER_TYPE eShaderType);


};

