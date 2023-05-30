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

/// ���������� ���¸� �����Ѵ�.
	COMMAND_QUEUE(CGameFramework)->GetCommandList()->SetPipelineState(DX12_PIPELINESTATE(CGameFramework)->GetPipelineState().Get());
/// ������Ƽ�� ��������(�ﰢ�� ����Ʈ)�� �����Ѵ�. 
	COMMAND_QUEUE(CGameFramework)->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	COMMAND_QUEUE(CGameFramework)->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}

void CGameObject::Update()
{

}

void CGameObject::FinalUpdate()
{

}