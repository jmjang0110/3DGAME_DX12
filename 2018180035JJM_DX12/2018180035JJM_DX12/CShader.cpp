#include "stdafx.h"
#include "CShader.h"

CShader::CShader()
{
}

CShader::~CShader()
{
}

BOOL CShader::OnCreate(SHADER_TYPE eShaderType,
						const LPCWSTR& wstrFileName,
						const LPCSTR& strFuncName,
						const LPCSTR& strShaderVersion)
{


	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	switch (eShaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
	{
		D3DCompileFromFile(wstrFileName, NULL, NULL, strFuncName, strShaderVersion, nCompileFlags, 0,
			m_VSBlob.GetAddressOf(), NULL);
	}
		break;
	case SHADER_TYPE::HULL_SHADER:
		break;
	case SHADER_TYPE::DOMAIN_SHADER:
		break;
	case SHADER_TYPE::GEOMETRY_SHADER:
		break;
	case SHADER_TYPE::PIXEL_SHADER:
	{
		D3DCompileFromFile(wstrFileName, NULL, NULL, strFuncName, strShaderVersion, nCompileFlags, 0,
			m_PSBlob.GetAddressOf(), NULL);
	}
		break;
	}


	return true;

}

ComPtr<ID3DBlob> CShader::GetBlob(SHADER_TYPE eShaderType)
{
	switch (eShaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
	{
		return m_VSBlob;
	}
		break;
	case SHADER_TYPE::HULL_SHADER:
		break;
	case SHADER_TYPE::DOMAIN_SHADER:
		break;
	case SHADER_TYPE::GEOMETRY_SHADER:
		break;
	case SHADER_TYPE::PIXEL_SHADER:
	{
		return m_PSBlob;
	}
		break;
	}

}