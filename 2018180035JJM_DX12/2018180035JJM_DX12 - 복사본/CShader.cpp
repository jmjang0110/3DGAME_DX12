#include "stdafx.h"
#include "CShader.h"

CShader::CShader()
{
}

CShader::~CShader()
{
}

BOOL CShader::OnCreate( SHADER_TYPE eShaderType,
						const LPCWSTR& wstrFileName,
						const LPCSTR& strFuncName,
						const LPCSTR& strShaderVersion)
{

	m_eType = eShaderType;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	switch (eShaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
	{
		HRESULT hResult = ::D3DCompileFromFile(wstrFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, strFuncName, strShaderVersion, nCompileFlags, 0, m_VSBlob.GetAddressOf(), m_ErrorBlob.GetAddressOf());
		char* pErrorString = NULL;
		if (m_ErrorBlob) 
			pErrorString = (char*)m_ErrorBlob->GetBufferPointer();

		if (m_VSBlob) {
		m_ShaderByteCode.BytecodeLength = m_VSBlob->GetBufferSize();
		m_ShaderByteCode.pShaderBytecode = m_VSBlob->GetBufferPointer();
		}


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
		D3DCompileFromFile(wstrFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, strFuncName, strShaderVersion, nCompileFlags, 0,
			m_PSBlob.GetAddressOf(), m_ErrorBlob.GetAddressOf());
	
		char* pErrorString = NULL;
		if (m_ErrorBlob)
			pErrorString = (char*)m_ErrorBlob->GetBufferPointer();

		if (m_PSBlob) {
			m_ShaderByteCode.BytecodeLength = m_PSBlob->GetBufferSize();
			m_ShaderByteCode.pShaderBytecode = m_PSBlob->GetBufferPointer();
		}

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

	return nullptr;

}

void CShader::CreateShaderVariables()
{
}

void CShader::UpdateShaderVariables()
{
}

void CShader::ReleaseShaderVariables()
{
}
