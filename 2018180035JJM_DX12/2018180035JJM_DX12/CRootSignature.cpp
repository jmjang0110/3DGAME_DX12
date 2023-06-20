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


	
	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera [ b1 ] 
	pd3dRootParameters[0].Descriptor.RegisterSpace  = 0;
	pd3dRootParameters[0].ShaderVisibility          = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject [ b2 ] 
	pd3dRootParameters[1].Constants.RegisterSpace  = 0;
	pd3dRootParameters[1].ShaderVisibility         = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights [ b4 ] 
	pd3dRootParameters[2].Descriptor.RegisterSpace  = 0;
	pd3dRootParameters[2].ShaderVisibility          = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	
	/// D3D12_ROOT_SIGNATURE_DESC
	//루트 시그너쳐를 생성한다. 
	::ZeroMemory(&m_RootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	m_RootSignatureDesc.NumParameters		= _countof(pd3dRootParameters); /// 루트 시그니처 슬롯 개수 
	m_RootSignatureDesc.pParameters			= pd3dRootParameters;			/// 
	m_RootSignatureDesc.NumStaticSamplers	= 0;							/// 정적 샘플러 개수 ( 2032개 )
	m_RootSignatureDesc.pStaticSamplers		= NULL;				
	m_RootSignatureDesc.Flags				= d3dRootSignatureFlags;		/// 루트 시그니처 레이아웃을 위한 선택 사항 

	
	::D3D12SerializeRootSignature(&m_RootSignatureDesc
								, D3D_ROOT_SIGNATURE_VERSION_1
								,&m_blobSignature
								, &m_blobError);
	
	auto pDevice = DEVICE(CGameFramework);
	pDevice->GetDevice()->CreateRootSignature(0
											, m_blobSignature->GetBufferPointer()
											, m_blobSignature->GetBufferSize()
											, __uuidof(ID3D12RootSignature)
											, (void**)m_RootSignature.GetAddressOf());

}