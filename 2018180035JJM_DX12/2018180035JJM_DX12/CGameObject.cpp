#include "stdafx.h"
#include "CGameObject.h"
#include "CGameFramework.h"

#include "CCommandQueue.h"
#include "CRootSignature.h"
#include "CGraphicsPipelineState.h"


CGameObject::CGameObject()
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::Render()
{

/// 파이프라인 상태를 설정한다.
	COMMAND_QUEUE(CGameFramework)->GetCommandList()->SetPipelineState(DX12_PIPELINESTATE(CGameFramework)->GetPipelineState().Get());
/// 프리미티브 토폴로지(삼각형 리스트)를 설정한다. 
	COMMAND_QUEUE(CGameFramework)->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	COMMAND_QUEUE(CGameFramework)->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}

void CGameObject::Update()
{

}

void CGameObject::FinalUpdate()
{

}