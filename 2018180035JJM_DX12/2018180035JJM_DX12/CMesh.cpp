#include "stdafx.h"
#include "CMesh.h"

#include "CCommandQueue.h"
#include "CGameFramework.h"
#include "ResourceManager.h"

#include "CVertex.h"
#include "CComponent.h"

CMesh::CMesh()
{

}

CMesh::~CMesh()
{

}

void CMesh::CreateVertexBufferResource(void* pData
	, UINT nDataCnt
	, UINT nStride
	, D3D12_HEAP_TYPE eHeapType
	, D3D12_RESOURCE_STATES eResState)
{
	m_nVertices = nDataCnt;

	m_pd3dVertexBuffer = ResourceManager::GetInst()->CreateBufferResource(
		pData,
		nStride * nDataCnt,
		eHeapType,
		eResState,
		&m_pd3dVertexUploadBuffer
	);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = nStride;
	m_d3dVertexBufferView.SizeInBytes = nStride * nDataCnt;

}

void CMesh::CreateIndexBufferResource(void* pData, UINT nDataCnt, UINT nStride, D3D12_HEAP_TYPE eHeapType, D3D12_RESOURCE_STATES eResState, DXGI_FORMAT eDxgiFormat)
{
	m_nIndices = nDataCnt;

	m_pd3dIndexBuffer = ResourceManager::GetInst()->CreateBufferResource(
		pData
		, nStride * nDataCnt
		, eHeapType
		, eResState
		, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format         = eDxgiFormat;
	m_d3dIndexBufferView.SizeInBytes    = nStride * nDataCnt;

}

void CMesh::CreateNormalBufferResource(void* pData, UINT nDataCnt, UINT nStride, D3D12_HEAP_TYPE eHeapType, D3D12_RESOURCE_STATES eResState)
{
	m_pd3dNormalBuffer = ResourceManager::GetInst()->CreateBufferResource(
		pData
		, nStride
		, eHeapType
		, eResState
		, &m_pd3dNormalUploadBuffer);

	m_d3dNormalBufferView.BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
	m_d3dNormalBufferView.StrideInBytes  = nStride;
	m_d3dNormalBufferView.SizeInBytes    = nStride;
}

void CMesh::CreateNormalBufferResource(MeshLoadInfo* pMeshInfo)
{
	m_pd3dNormalBuffer = ResourceManager::GetInst()->CreateBufferResource(pMeshInfo->m_pxmf3Normals
													, sizeof(XMFLOAT3) * pMeshInfo->m_nVertices
													, D3D12_HEAP_TYPE_DEFAULT
													, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
													, &m_pd3dNormalUploadBuffer);

	m_d3dNormalBufferView.BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
	m_d3dNormalBufferView.StrideInBytes  = sizeof(XMFLOAT3);
	m_d3dNormalBufferView.SizeInBytes    = sizeof(XMFLOAT3) * pMeshInfo->m_nVertices;
}

void CMesh::CreateIndexBufferResource(MeshLoadInfo* pMeshInfo)
{


	m_nIndices = pMeshInfo->m_nIndices;

	m_pd3dIndexBuffer = ResourceManager::GetInst()->CreateBufferResource(pMeshInfo->m_pnIndices
		, sizeof(UINT) * m_nIndices
		, D3D12_HEAP_TYPE_DEFAULT
		, D3D12_RESOURCE_STATE_INDEX_BUFFER
		, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format         = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes    = sizeof(UINT) * m_nIndices;

}

void CMesh::CreateIndexBufferResource(int nIndices, UINT* pnIndices)
{
	m_nIndices = nIndices;

	m_pd3dIndexBuffer = ResourceManager::GetInst()->CreateBufferResource(pnIndices
		, sizeof(UINT) * m_nIndices
		, D3D12_HEAP_TYPE_DEFAULT
		, D3D12_RESOURCE_STATE_INDEX_BUFFER
		, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;

}

void CMesh::CreateVertexBufferResource(int nVertices, XMFLOAT3* pnVertices)
{
	m_nVertices = nVertices;

	m_pd3dVertexBuffer = ResourceManager::GetInst()->CreateBufferResource(
		pnVertices,
		sizeof(XMFLOAT3) * nVertices,
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&m_pd3dVertexUploadBuffer
	);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = sizeof(XMFLOAT3);
	m_d3dVertexBufferView.SizeInBytes = sizeof(XMFLOAT3) * nVertices;

}

void CMesh::OnCreate(int verticesNum, int nStride, D3D12_PRIMITIVE_TOPOLOGY topology, std::vector<CVertex> vertices)
{

	m_nVertices            = verticesNum;
	m_nStride              = nStride;
	m_d3dPrimitiveTopology = topology;
	
	m_Vertices             = vertices;

	/// 메쉬를 리소스 ( 정점 버퍼 ) 로 생성한다. 
	m_pd3dVertexBuffer = ResourceManager::GetInst()->CreateBufferResource((void*)&m_Vertices
																				, m_nStride * m_nVertices
																				, D3D12_HEAP_TYPE_DEFAULT
																				, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
																				, &m_pd3dVertexUploadBuffer);

	/// 정점 버퍼 뷰를 생성한다. 
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes  = m_nStride;
	m_d3dVertexBufferView.SizeInBytes    = m_nStride * m_nVertices;

}

void CMesh::OnCreate(MeshLoadInfo* tMeshInfo)
{
	m_tMeshInfo = *tMeshInfo;
	m_nVertices = m_tMeshInfo.m_nVertices;
	m_nType = m_tMeshInfo.m_nType;

	m_pd3dVertexBuffer = ResourceManager::GetInst()->CreateBufferResource(m_tMeshInfo.m_pxmf3Positions
		, sizeof(XMFLOAT3) * m_nVertices
		, D3D12_HEAP_TYPE_DEFAULT
		, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		, &m_pd3dVertexUploadBuffer);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = sizeof(XMFLOAT3);
	m_d3dVertexBufferView.SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_nSubMeshes = m_tMeshInfo.m_nSubMeshes;
	if (m_nSubMeshes > 0)
	{
		for (int i = 0; i < m_nSubMeshes; i++)
		{
			ComPtr<ID3D12Resource>	pIdxBuffer{};
			ComPtr<ID3D12Resource>	pIdxUploadBuffer{};
			D3D12_INDEX_BUFFER_VIEW pIdxBufferView{};

			int indice = m_tMeshInfo.m_pnSubSetIndices[i];
			m_pnSubSetIndices.push_back(indice);

			pIdxBuffer = ResourceManager::GetInst()->CreateBufferResource(m_tMeshInfo.m_ppnSubSetIndices[i]
																		, sizeof(UINT) * m_tMeshInfo.m_pnSubSetIndices[i]
																		, D3D12_HEAP_TYPE_DEFAULT
																		, D3D12_RESOURCE_STATE_INDEX_BUFFER
																		, &pIdxUploadBuffer);
			m_pd3dSubSet_IndexBuffers.push_back(pIdxBuffer);
			m_pd3dSubSet_IndexUploadBuffers.push_back(pIdxUploadBuffer);

			pIdxBufferView.BufferLocation = m_pd3dSubSet_IndexBuffers[i]->GetGPUVirtualAddress(); /// 버퍼 리소스의 GPU 주소 반환 
			pIdxBufferView.Format         = DXGI_FORMAT_R32_UINT;
			pIdxBufferView.SizeInBytes    = sizeof(UINT) * m_tMeshInfo.m_pnSubSetIndices[i];

			m_pd3dSubSet_IndexBufferViews.push_back(pIdxBufferView);
		}
	}
}




void CMesh::Render(int nSubSet)
{
	auto pCmdList = DX12_COMMAND_LIST;
	pCmdList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	if(m_pd3dNormalBuffer)
	{
		D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[2] = { m_d3dVertexBufferView, m_d3dNormalBufferView };
		pCmdList->IASetVertexBuffers(m_nSlot, 2, pVertexBufferViews);
	}
	else 
	{
		D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[1] = { m_d3dVertexBufferView };
		pCmdList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	}

	if ((m_nSubMeshes > 0) && (nSubSet < m_nSubMeshes))
	{
		pCmdList->IASetIndexBuffer(&(m_pd3dSubSet_IndexBufferViews[nSubSet]));
		pCmdList->DrawIndexedInstanced(m_pnSubSetIndices[nSubSet], 1, 0, 0, 0);
		
			/// DrawIndexedInstanced( 인덱스 개수
			///						, 인스턴스 개수
			///						, 시작 인덱스 위치
			///						, 각 정점 인덱스에 더해지는 값
			///						, 인스턴스 인덱스에 더해지는 값 )	
	}
	else
	{
		if(!m_pd3dIndexBuffer)
			pCmdList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
		else
		{
			pCmdList->IASetIndexBuffer(&(m_d3dIndexBufferView));
			pCmdList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
		}
	}
}

void CMesh::ReleaseUploadBuffers()
{

	// 정점 버퍼를 위한 업로드 버퍼를 소멸시킨다. 
	if (m_pd3dVertexUploadBuffer)
		m_pd3dVertexUploadBuffer->Release();

	//m_pd3dVertexUploadBuffer = nullptr;

}

