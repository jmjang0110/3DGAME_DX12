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
	auto pCmdList = COMMAND_LIST(CGameFramework);

	pCmdList->SetGraphicsRoot32BitConstants(1, 4, &m_tColor.m_xmf4Ambient, 16);
	pCmdList->SetGraphicsRoot32BitConstants(1, 4, &m_tColor.m_xmf4Diffuse, 20);
	pCmdList->SetGraphicsRoot32BitConstants(1, 4, &m_tColor.m_xmf4Specular, 24);
	pCmdList->SetGraphicsRoot32BitConstants(1, 4, &m_tColor.m_xmf4Emissive, 28);

}

void CMaterial::OnPrepareRender()
{
	auto pCmdList = COMMAND_LIST(CGameFramework);
	if (m_PipelineState)
		pCmdList->SetPipelineState(m_PipelineState->GetPipelineState().Get());



}

