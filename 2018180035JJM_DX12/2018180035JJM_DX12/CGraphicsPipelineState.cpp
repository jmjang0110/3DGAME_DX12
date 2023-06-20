#include "stdafx.h"
#include "CGraphicsPipelineState.h"
#include "CGameFramework.h"
#include "CDevice.h"
#include "CShader.h"
#include "CRootSignature.h"
#include "ResourceManager.h"
#include "CGameObject.h"

#include "CCommandQueue.h"

CGraphicsPipelineState::CGraphicsPipelineState() 
{


}

CGraphicsPipelineState::~CGraphicsPipelineState()
{
}

HRESULT CGraphicsPipelineState::CreateGraphicsPipelineState()
{

	auto pRootSignature = ROOT_SIGNATURE(CGameFramework);
	
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
		m_pipelineDesc.PS.BytecodeLength  = m_PS->GetBlob(SHADER_TYPE::PIXEL_SHADER)->GetBufferSize();
	}


	m_pipelineDesc.RasterizerState					= m_d3dRasterizerDesc;
	m_pipelineDesc.BlendState						= m_d3dBlendDesc;
	m_pipelineDesc.DepthStencilState				= m_d3dDepthStencilDesc;
	m_pipelineDesc.InputLayout						= m_d3dInputLayoutDesc;

	m_pipelineDesc.SampleMask						= UINT_MAX;
	m_pipelineDesc.PrimitiveTopologyType			= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_pipelineDesc.NumRenderTargets					= 1;
	m_pipelineDesc.RTVFormats[0]					= DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pipelineDesc.DSVFormat						= DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_pipelineDesc.SampleDesc.Count					= 1;
	m_pipelineDesc.Flags							= D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = DEVICE(CGameFramework)->GetDevice()->CreateGraphicsPipelineState(&m_pipelineDesc
											,__uuidof(ID3D12PipelineState)
											, (void**)m_pd3dPipelineState.GetAddressOf());



	if (m_pipelineDesc.InputLayout.pInputElementDescs)
		delete[] m_pipelineDesc.InputLayout.pInputElementDescs;

	return hResult;

}

void CGraphicsPipelineState::SetPipelineState()
{
	if (m_pd3dPipelineState)
		COMMAND_LIST(CGameFramework)->SetPipelineState(m_pd3dPipelineState.Get());


}

void CGraphicsPipelineState::PushBack_GameObject(CGameObject* pObj)
{
	m_vecGameObjects.push_back(pObj);

}

void CGraphicsPipelineState::Clear_GameObject()
{
	m_vecGameObjects.clear();
}



void CGraphicsPipelineState::ExecuteRender()
{
	for (CGameObject* pGameObj : m_vecGameObjects)
	{
		pGameObj->Render();
	}

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
	CreateGraphicsPipelineState();
}