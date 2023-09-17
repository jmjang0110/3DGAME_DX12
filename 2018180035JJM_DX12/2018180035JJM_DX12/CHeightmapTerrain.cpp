
#include "stdafx.h"
#include "CHeightmapTerrain.h"

#include "CHeightMapImage.h"
#include "CHeightmapGrid.h"
#include "CMesh.h"
#include "ResourceManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CGameFramework.h"
#include "CScene.h"


#include "Resource.h"
#include "ResourceManager.h"

CHeightmapTerrain::CHeightmapTerrain(
	LPCTSTR pFileName
	, int nWidth
	, int nLength
	, int nBlockWidth
	, int nBlockLength
	, XMFLOAT3 xmf3Scale
	, XMFLOAT4 xmf4Color)
{
	m_pTerrainObj = new CGameObject;
	CTransform* pTrans = new CTransform;
	pTrans->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
	m_pTerrainObj->AddComponent(pTrans);


	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;
	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. 
	nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크
	기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.
	*/

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;
	//xmf3Scale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다. 
	m_xmf3Scale = xmf3Scale;
	//지형에 사용할 높이 맵을 생성한다. 
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);
	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다. 
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다. 
	int nMeshes = cxBlocks * czBlocks;
	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
	
	//m_ppMeshes = new CMesh*[nMeshes];

	//for (int i = 0; i < nMeshes; i++)m_ppMeshes[i] = NULL;
	//CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다. 
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			std::shared_ptr<CMesh> pGridMesh = ResourceManager::GetInst()->CreateGridTerrainMesh(
				z * 10 + x,
				xStart,
				zStart,
				nBlockWidth,
				nBlockLength,
				xmf3Scale,
				m_pHeightMapImage);

			pGridMesh->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//pHeightMapGridMesh = new CHeightMapGridMesh(xStart,
			//	zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			m_pTerrainObj->SetMesh(pGridMesh);
			m_pTerrainObj->SetMaterial(z * 10 + x, ResourceManager::GetInst()->GetMaterial("skin0"));

		}
	}
}

CHeightmapTerrain::~CHeightmapTerrain()
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}
