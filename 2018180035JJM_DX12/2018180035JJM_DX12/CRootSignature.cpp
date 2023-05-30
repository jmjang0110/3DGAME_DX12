#include "stdafx.h"
#include "CRootSignature.h"
#include "CDevice.h"
#include "CGameFramework.h"


CRootSignature::CRootSignature()
{
}

CRootSignature::~CRootSignature()
{
}

void CRootSignature::OnCreate()
{
	//루트 시그너쳐를 생성한다. 
	::ZeroMemory(&m_RootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	m_RootSignatureDesc.NumParameters     = 0;
	m_RootSignatureDesc.pParameters       = NULL;
	m_RootSignatureDesc.NumStaticSamplers = 0; 
	m_RootSignatureDesc.pStaticSamplers   = NULL;
	m_RootSignatureDesc.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	::D3D12SerializeRootSignature(&m_RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,&m_blobSignature, &m_blobError);
	
	auto pDevice = DEVICE(CGameFramework);
	pDevice->GetDevice()->CreateRootSignature(0, m_blobSignature->GetBufferPointer(),
		m_blobSignature->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)m_RootSignature.GetAddressOf());

	if (m_blobSignature) m_blobSignature->Release();
	if (m_blobError) m_blobError->Release();

}