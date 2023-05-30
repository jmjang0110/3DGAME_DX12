#include "stdafx.h"
#include "CGraphicsPipelineState.h"
#include "CGameFramework.h"
#include "CDevice.h"
#include "CShader.h"
#include "CRootSignature.h"


CGraphicsPipelineState::CGraphicsPipelineState() 
{

}

CGraphicsPipelineState::~CGraphicsPipelineState()
{

}

void CGraphicsPipelineState::SetRasterizerState() 
{
/// 래스터라이저 상태를 설정한다. 
	::ZeroMemory(&m_d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	m_d3dRasterizerDesc.FillMode              = D3D12_FILL_MODE_SOLID;
	m_d3dRasterizerDesc.CullMode              = D3D12_CULL_MODE_BACK;
	m_d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	m_d3dRasterizerDesc.DepthBias             = 0;
	m_d3dRasterizerDesc.DepthBiasClamp        = 0.0f;
	m_d3dRasterizerDesc.SlopeScaledDepthBias  = 0.0f;
	m_d3dRasterizerDesc.DepthClipEnable       = TRUE;
	m_d3dRasterizerDesc.MultisampleEnable     = FALSE;
	m_d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	m_d3dRasterizerDesc.ForcedSampleCount     = 0;
	m_d3dRasterizerDesc.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

void CGraphicsPipelineState::SetBlendState() 
{
/// 블렌드 상태를 설정한다. 
	::ZeroMemory(&m_d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	m_d3dBlendDesc.AlphaToCoverageEnable                 = FALSE;
	m_d3dBlendDesc.IndependentBlendEnable                = FALSE;
	m_d3dBlendDesc.RenderTarget[0].BlendEnable           = FALSE;
	m_d3dBlendDesc.RenderTarget[0].LogicOpEnable         = FALSE; 
	m_d3dBlendDesc.RenderTarget[0].SrcBlend              = D3D12_BLEND_ONE;
	m_d3dBlendDesc.RenderTarget[0].DestBlend             = D3D12_BLEND_ZERO;
	m_d3dBlendDesc.RenderTarget[0].BlendOp               = D3D12_BLEND_OP_ADD;
	m_d3dBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D12_BLEND_ONE;
	m_d3dBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D12_BLEND_ZERO;
	m_d3dBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D12_BLEND_OP_ADD;
	m_d3dBlendDesc.RenderTarget[0].LogicOp               = D3D12_LOGIC_OP_NOOP;
	m_d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


}

void CGraphicsPipelineState::SetGraphicsPipelineState()
{

	auto pRootSignature = ROOT_SIGNATURE(CGameFramework)->GetRootSignature();
	
	//그래픽 파이프라인 상태를 설정한다.
	::ZeroMemory(&m_pipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	m_pipelineDesc.pRootSignature                  = pRootSignature.Get();

/// SETTING VERTEX SHADER 
	if (m_VS.get() != nullptr) {
		m_pipelineDesc.VS.pShaderBytecode = m_VS->GetBlob(SHADER_TYPE::VERTEX_SHADER)->GetBufferPointer();
		m_pipelineDesc.VS.BytecodeLength  = m_VS->GetBlob(SHADER_TYPE::VERTEX_SHADER)->GetBufferSize();
	}


/// SETTING PIXEL SHADER 
	if (m_PS.get() != nullptr) {
		m_pipelineDesc.PS.pShaderBytecode = m_PS->GetBlob(SHADER_TYPE::PIXEL_SHADER)->GetBufferPointer();
		m_pipelineDesc.PS.BytecodeLength = m_PS->GetBlob(SHADER_TYPE::PIXEL_SHADER)->GetBufferSize();
	}


	m_pipelineDesc.RasterizerState                 = m_d3dRasterizerDesc;
	m_pipelineDesc.BlendState                      = m_d3dBlendDesc;
	m_pipelineDesc.DepthStencilState.DepthEnable   = FALSE;
	m_pipelineDesc.DepthStencilState.StencilEnable = FALSE;
	m_pipelineDesc.InputLayout.pInputElementDescs  = NULL;
	m_pipelineDesc.InputLayout.NumElements         = 0;
	m_pipelineDesc.SampleMask                      = UINT_MAX;
	m_pipelineDesc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_pipelineDesc.NumRenderTargets                = 1;
	m_pipelineDesc.RTVFormats[0]                   = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pipelineDesc.DSVFormat                       = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_pipelineDesc.SampleDesc.Count                = 1;
	m_pipelineDesc.SampleDesc.Quality              = 0;

	DEVICE(CGameFramework)->GetDevice()->CreateGraphicsPipelineState(&m_pipelineDesc
											,__uuidof(ID3D12PipelineState)
											, (void**)m_pd3dPipelineState.GetAddressOf());

}


void CGraphicsPipelineState::SetVertexShader(std::shared_ptr<CShader> vs)
{
	m_VS = vs;

}

void CGraphicsPipelineState::SetPixelShader(std::shared_ptr<CShader> ps)
{
	m_PS = ps;

}


void CGraphicsPipelineState::OnCreate()
{
	SetRasterizerState();
	SetBlendState();

	m_VS = std::make_shared<CShader>();
	m_PS = std::make_shared<CShader>();

	m_VS->OnCreate(SHADER_TYPE::VERTEX_SHADER, L"VertexShader.hlsl", "VS_Main", "vs_5_1");
	m_PS->OnCreate(SHADER_TYPE::PIXEL_SHADER, L"PixelShader.hlsl", "PS_Main", "ps_5_1");

	SetGraphicsPipelineState();

}