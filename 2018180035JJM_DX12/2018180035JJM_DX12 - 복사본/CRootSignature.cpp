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


	D3D12_ROOT_PARAMETER pd3dRootParameters[4];

	/// [ b0 ]
	pd3dRootParameters[0].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 0; //Camera [ b0 ] 
	pd3dRootParameters[0].Descriptor.RegisterSpace  = 0;
	pd3dRootParameters[0].ShaderVisibility          = D3D12_SHADER_VISIBILITY_ALL;

	/// [ b1 ]
	pd3dRootParameters[1].ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1; //GameObject [ b1 ] 
	pd3dRootParameters[1].Constants.RegisterSpace  = 0;
	pd3dRootParameters[1].ShaderVisibility         = D3D12_SHADER_VISIBILITY_ALL;

	/// [ b2 ]
	pd3dRootParameters[2].ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 2; //Lights [ b2 ] 
	pd3dRootParameters[2].Descriptor.RegisterSpace  = 0;
	pd3dRootParameters[2].ShaderVisibility          = D3D12_SHADER_VISIBILITY_ALL;

	/// [ t0 ]  
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges_t0{};
	pd3dDescriptorRanges_t0.RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges_t0.NumDescriptors                    = 1;
	pd3dDescriptorRanges_t0.BaseShaderRegister                = 0; //register(t0) : gtxtTexture -> HLSL 
	pd3dDescriptorRanges_t0.RegisterSpace                     = 0; //register(t0, space0) -> HLSL 
	pd3dDescriptorRanges_t0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dRootParameters[3].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges   = &pd3dDescriptorRanges_t0;
	pd3dRootParameters[3].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_PIXEL;

	/// [ s0 ]
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc{};
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter           = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias       = 0;
	d3dSamplerDesc.MaxAnisotropy    = 1;
	d3dSamplerDesc.ComparisonFunc   = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD           = 0;
	d3dSamplerDesc.MaxLOD           = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister   = 0;
	d3dSamplerDesc.RegisterSpace    = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS 
													| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	
	/// D3D12_ROOT_SIGNATURE_DESC
	//루트 시그너쳐를 생성한다. 
	::ZeroMemory(&m_RootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	m_RootSignatureDesc.NumParameters		= _countof(pd3dRootParameters); /// 루트 시그니처 슬롯 개수 
	m_RootSignatureDesc.pParameters			= pd3dRootParameters;			/// 
	m_RootSignatureDesc.NumStaticSamplers	= 1;							/// 정적 샘플러 개수 ( 2032개 )
	m_RootSignatureDesc.pStaticSamplers		= &d3dSamplerDesc;
	m_RootSignatureDesc.Flags				= d3dRootSignatureFlags;		/// 루트 시그니처 레이아웃을 위한 선택 사항 

	
	::D3D12SerializeRootSignature(&m_RootSignatureDesc
								, D3D_ROOT_SIGNATURE_VERSION_1
								, &m_blobSignature
								, &m_blobError);
	
	auto pDevice = DX12_DEVICE;
	pDevice->GetDevice()->CreateRootSignature(0
											, m_blobSignature->GetBufferPointer()
											, m_blobSignature->GetBufferSize()
											, __uuidof(ID3D12RootSignature)
											, (void**)m_RootSignature.GetAddressOf());

}