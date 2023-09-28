#include "stdafx.h"
#include "CMaterial.h"

#include "CGameFramework.h"
#include "CCommandQueue.h"
#include "CGraphicsPipelineState.h"


CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
	m_PipelineState.reset();

}


void CMaterial::UpdateShaderVariable()
{

	/// SetGraphicsRoot32BitConstants( 
	DX12_COMMAND_LIST->SetGraphicsRoot32BitConstants((UINT)CBV_REGISTER::b1, 4, &m_tColor.m_xmf4Ambient, 16);
	DX12_COMMAND_LIST->SetGraphicsRoot32BitConstants((UINT)CBV_REGISTER::b1, 4, &m_tColor.m_xmf4Diffuse, 20);
	DX12_COMMAND_LIST->SetGraphicsRoot32BitConstants((UINT)CBV_REGISTER::b1, 4, &m_tColor.m_xmf4Specular, 24);
	DX12_COMMAND_LIST->SetGraphicsRoot32BitConstants((UINT)CBV_REGISTER::b1, 4, &m_tColor.m_xmf4Emissive, 28);

}

void CMaterial::OnPrepareRender()
{
	auto pCmdList = DX12_COMMAND_LIST;
	if (m_PipelineState)
		pCmdList->SetPipelineState(m_PipelineState->GetPipelineState().Get());



}

