
#include "stdafx.h"
#include "CFileManager.h"

#include "CGameFramework.h"
#include "CDevice.h"
#include "CCommandQueue.h"
#include "CGraphicsPipelineState.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CTransform.h"

#include "ResourceManager.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTransform.h"
#include "CScene.h"




CFileManager* CFileManager::m_pInst = nullptr;

int CFileManager::ReadIntegerFromFile(FILE* pInFile)
{
	int nValue = 0;
	UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile);
	return(nValue);
}

float CFileManager::ReadFloatFromFile(FILE* pInFile)
{
	float fValue = 0;
	UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile);
	return(fValue);
}

BYTE CFileManager::ReadStringFromFile(FILE* pInFile, char* pstrToken)
{
	BYTE nStrLength = 0;
	UINT nReads = 0;
	nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
	pstrToken[nStrLength] = '\0';

	return(nStrLength);
}

void CFileManager::PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	//if (pGameObject->m_pSibling) 
	//	PrintFrameInfo(pGameObject->m_pSibling, pParent);

	auto Childs = pGameObject->GetChild();
	for(CGameObject* pChild : Childs)
		PrintFrameInfo(pChild, pGameObject);


}
MeshLoadInfo* CFileManager::LoadFromFile_2(const char* pstrFileName)
{
	MeshLoadInfo* pMeshInfo = new MeshLoadInfo;

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[256] = { '\0' };

	BYTE nStrLength = 0;
	UINT nReads = 0;
	int nPositions = 0, nColors = 0, nNormals = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	::ReadUnityBinaryString(pInFile, pstrToken, &nStrLength); //"<BoundingBox>:"
	nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBCenter), sizeof(XMFLOAT3), 1, pInFile);
	nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBExtents), sizeof(XMFLOAT3), 1, pInFile);

	
	::ReadUnityBinaryString(pInFile, pstrToken, &nStrLength); //"<Positions>:"
	nPositions = ReadIntegerFromFile(pInFile);
	pMeshInfo->m_nVertices = nPositions;
	if (nPositions > 0)
	{
		pMeshInfo->m_nType |= VERTEXT_POSITION;
		pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
		nReads = (UINT)::fread(pMeshInfo->m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
	}

	
	::ReadUnityBinaryString(pInFile, pstrToken, &nStrLength); //"<Normals>:"
	nNormals = ReadIntegerFromFile(pInFile);
	if (nNormals > 0)
	{
		pMeshInfo->m_nType |= VERTEXT_NORMAL;
		pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
		nReads = (UINT)::fread(pMeshInfo->m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
	}
	
	::ReadUnityBinaryString(pInFile, pstrToken, &nStrLength); //"<TextureCoords>:"
	int nVertices = ReadIntegerFromFile(pInFile);
	if (nVertices > 0)
	{
		//pMeshInfo->m_nType |= VERTEXT_POSITION;
		XMFLOAT2* TexCoords = new XMFLOAT2[nVertices];
		int nReads = (UINT)::fread(TexCoords, sizeof(XMFLOAT2), nVertices, pInFile);
	}
	
	::ReadUnityBinaryString(pInFile, pstrToken, &nStrLength); //"<Indices>:"
	nIndices = ReadIntegerFromFile(pInFile);
	pMeshInfo->m_nIndices = nIndices;
	if (nIndices > 0)
	{
		pMeshInfo->m_pnIndices = new UINT[nIndices];
		nReads = (UINT)::fread(pMeshInfo->m_pnIndices, sizeof(UINT), nIndices, pInFile);

	}
	::fclose(pInFile);

	return pMeshInfo;

}
#define _WITH_DEBUG_FRAME_HIERARCHY

MATERIALSLOADINFO* CFileManager::LoadMaterialsInfoFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nMaterial = 0;

	MATERIALSLOADINFO* pMaterialsInfo = new MATERIALSLOADINFO;

	pMaterialsInfo->m_nMaterials = ReadIntegerFromFile(pInFile);
	pMaterialsInfo->m_pMaterials = new MATERIALLOADINFO[pMaterialsInfo->m_nMaterials];

	for (; ; )
	{
		ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Material>:"))
		{
			nMaterial = ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossiness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fSmoothness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fSpecularHighlight), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fMetallic), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossyReflection), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
	return(pMaterialsInfo);
}

MeshLoadInfo* CFileManager::LoadMeshInfoFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nPositions = 0, nColors = 0, nNormals = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	MeshLoadInfo* pMeshInfo = new MeshLoadInfo;

	pMeshInfo->m_nVertices = ReadIntegerFromFile(pInFile);
	ReadStringFromFile(pInFile, pMeshInfo->m_pstrMeshName);

	for (; ; )
	{
		ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBCenter), sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBExtents), sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nPositions = ReadIntegerFromFile(pInFile);
			if (nPositions > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_POSITION;
				pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nColors = ReadIntegerFromFile(pInFile);
			if (nColors > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_COLOR;
				pMeshInfo->m_pxmf4Colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf4Colors, sizeof(XMFLOAT4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nNormals = ReadIntegerFromFile(pInFile);
			if (nNormals > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_NORMAL;
				pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Indices>:"))
		{
			nIndices = ReadIntegerFromFile(pInFile);
			if (nIndices > 0)
			{
				pMeshInfo->m_pnIndices = new UINT[nIndices];
				nReads = (UINT)::fread(pMeshInfo->m_pnIndices, sizeof(int), nIndices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			pMeshInfo->m_nSubMeshes = ReadIntegerFromFile(pInFile);
			if (pMeshInfo->m_nSubMeshes > 0)
			{
				pMeshInfo->m_pnSubSetIndices = new int[pMeshInfo->m_nSubMeshes];
				pMeshInfo->m_ppnSubSetIndices = new UINT * [pMeshInfo->m_nSubMeshes];
				for (int i = 0; i < pMeshInfo->m_nSubMeshes; i++)
				{
					ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = ReadIntegerFromFile(pInFile);
						pMeshInfo->m_pnSubSetIndices[i] = ReadIntegerFromFile(pInFile);
						if (pMeshInfo->m_pnSubSetIndices[i] > 0)
						{
							pMeshInfo->m_ppnSubSetIndices[i] = new UINT[pMeshInfo->m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(pMeshInfo->m_ppnSubSetIndices[i], sizeof(int), pMeshInfo->m_pnSubSetIndices[i], pInFile);
						}

					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
	return(pMeshInfo);
}

std::vector<CGameObject*> CFileManager::LoadSceneObjectsFromFile(char* pstrFileName)
{
	FILE* pFile = NULL;
	::fopen_s(&pFile, pstrFileName, "rb");
	::rewind(pFile);

	char pstrToken[256] = { '\0' };
	char pstrGameObjectName[256] = { '\0' };

	UINT nReads = 0;
	BYTE nStrLength = 0, nObjectNameLength = 0;
	int nObjects = 0;

	::ReadUnityBinaryString(pFile, pstrToken, &nStrLength); //"<GameObjects>:"
	nReads = (UINT)::fread(&nObjects, sizeof(int), 1, pFile);

	std::vector<CGameObject*> ppObjs;

	CGameObject* pGameObject = NULL;
	for (int i = 0; i < nObjects; i++)
	{
		pGameObject = new CGameObject();

		::ReadUnityBinaryString(pFile, pstrToken, &nStrLength); //"<GameObject>:"
		::ReadUnityBinaryString(pFile, pstrGameObjectName, &nObjectNameLength);
		pstrGameObjectName[nObjectNameLength] = '\0';
		char name[256] = { '\0'};
		strcpy_s(name, 256, pstrGameObjectName);

		XMFLOAT4X4 World;
		nReads = (UINT)::fread(&World, sizeof(float), 16, pFile); //Transform
		CTransform* TransCom = new CTransform;
		TransCom->SetPosition(World._41, World._42, World._42);
		std::shared_ptr<CMesh> pMesh = nullptr;
		for (int j = 0; j < i; j++)
		{
			if (pstrGameObjectName == ppObjs[j]->GetName())
			{
				pMesh = ppObjs[j]->GetMesh();
				break;
			}
		}
		if (!pMesh)
		{
			pMesh = std::make_shared<CMesh>();
			::ReadUnityBinaryString(pFile, pstrToken, &nStrLength); //"<Mesh>:"
			::ReadUnityBinaryString(pFile, pstrToken, &nStrLength); //"<Name>:"

			char pstrFilePath[256] = { '\0' };
			std::string filePath{};
			filePath = "Model/Meshes/";
			filePath += pstrGameObjectName;
			filePath += ".bin";

			const char* pstr = filePath.c_str();
			MeshLoadInfo* pMeshInfo = LoadFromFile_2(pstr);
			pMesh->OnCreate(pMeshInfo);
			pMesh->CreateIndexBufferResource(pMeshInfo);
			pMesh->CreateNormalBufferResource(pMeshInfo);

			//pMesh = new CMesh(pd3dDevice, pd3dCommandList, pstrFilePath);
		}

		pGameObject->SetName(pstrGameObjectName);
		pGameObject->SetMesh(pMesh);
		pGameObject->AddComponent(TransCom);

		ppObjs.push_back(pGameObject);
	}

	::fclose(pFile);

	return ppObjs;
}

CGameObject* CFileManager::LoadFrameHierarchyFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0;

	CGameObject* pGameObject = nullptr;

	for (; ; )
	{
		ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			pGameObject = new CGameObject();
			
			CTransform* TransCom = new CTransform;
			TransCom->SetPosition(0.f, 0.f, 0.f);
			pGameObject->AddComponent(TransCom);

			nFrame = ReadIntegerFromFile(pInFile);
			ReadStringFromFile(pInFile, pGameObject->GetFrameName());
			pGameObject->SetName(pGameObject->GetFrameName());

		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			XMFLOAT4X4 xmf4x4Transform{};
			nReads = (UINT)::fread(&xmf4x4Transform, sizeof(float), 16, pInFile);

		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			MeshLoadInfo* pMeshInfo = LoadMeshInfoFromFile(pInFile);
			if (pMeshInfo)
			{
				std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
				if (pMeshInfo->m_nType & VERTEXT_NORMAL)
				{
					pMesh = ResourceManager::GetInst()->GetMesh(pMeshInfo->m_pstrMeshName);
					
					if (pMesh == nullptr) {
						pMesh = std::make_shared<CMesh>();
						pMesh->OnCreate(pMeshInfo);
						pMesh->CreateNormalBufferResource(pMeshInfo);

						pMesh->m_Name = pMeshInfo->m_pstrMeshName;
						ResourceManager::GetInst()->AddMesh(pMeshInfo->m_pstrMeshName,pMesh);
						auto iter = ResourceManager::GetInst();
						int i = 0;
	

					}
				}

				if (pMesh) 
					pGameObject->SetMesh(pMesh);

				delete pMeshInfo;
			}
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			MATERIALSLOADINFO* pMaterialsInfo = LoadMaterialsInfoFromFile(pInFile);

			if (pMaterialsInfo && (pMaterialsInfo->m_nMaterials > 0))
			{
				//pGameObject->m_nMaterials = pMaterialsInfo->m_nMaterials;
				//pGameObject->m_ppMaterials = new CMaterial * [pMaterialsInfo->m_nMaterials];

				for (int i = 0; i < pMaterialsInfo->m_nMaterials; i++)
				{
					//pGameObject->m_ppMaterials[i] = NULL;

					std::shared_ptr<CMaterial> pMaterial = std::make_shared< CMaterial>();
					MATERIAL_COLOR mtrlColor{};
					mtrlColor.m_xmf4Diffuse          = pMaterialsInfo->m_pMaterials[i].m_xmf4AlbedoColor;
					mtrlColor.m_xmf4Specular         = pMaterialsInfo->m_pMaterials[i].m_xmf4SpecularColor; //(r,g,b,a=power)
					mtrlColor.m_xmf4Specular.w       = pMaterialsInfo->m_pMaterials[i].m_fGlossiness * 255.0f;
					mtrlColor.m_xmf4Emissive         = pMaterialsInfo->m_pMaterials[i].m_xmf4EmissiveColor;

					pMaterial->SetColor(mtrlColor);

					auto pObjMesh = pGameObject->GetMesh();
					if (pObjMesh != nullptr) {
						if (pObjMesh->GetType() & VERTEXT_NORMAL) {
							auto PS = ResourceManager::GetInst()->GetPipelineState("Illuminate");
							pMaterial->SetPipelineState(PS);
						}
					}
					
					pGameObject->SetMaterial(i, pMaterial);

					std::string MtrlName = pGameObject->GetName();
					MtrlName = MtrlName + std::to_string(i);
					pMaterial->m_Name = MtrlName;
					ResourceManager::GetInst()->AddMaterial(MtrlName, pMaterial);

				}
			}
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					CGameObject* pChild = LoadFrameHierarchyFromFile(pInFile);
					if (pChild) 
						pGameObject->AddChild(pChild);
#ifdef _WITH_DEBUG_RUNTIME_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, _T("(Child Frame: %p) (Parent Frame: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
}

CGameObject* CFileManager::LoadGeometryFromFile(std::string pstrFileName)
{
	const char* FileName = &pstrFileName[0];
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	CGameObject* pGameObject = NULL;
	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Hierarchy>:"))
		{
			pGameObject = LoadFrameHierarchyFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "</Hierarchy>"))
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	PrintFrameInfo(pGameObject, NULL);
#endif

	return(pGameObject);
}

MeshLoadInfo* CFileManager::LoadMeshInfo_Tank(std::string pstrFileName)
{
	const char* FileName = &pstrFileName[0];
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nPositions = 0, nColors = 0, nNormals = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	MeshLoadInfo* pMeshInfo = new MeshLoadInfo{};

	for (; ; )
	{
		ReadStringFromFile(pInFile, pstrToken);


		if (!strcmp(pstrToken, "<BoundingBox>:"))
		{
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBCenter), sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBExtents), sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Vertices>:"))
		{
			nPositions = ReadIntegerFromFile(pInFile);
			pMeshInfo->m_nVertices = nPositions;
			if (nPositions > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_POSITION;
				pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nNormals = ReadIntegerFromFile(pInFile);
			if (nNormals > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_NORMAL;
				pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords>:"))
		{
			int nVertices = ReadIntegerFromFile(pInFile);
			if (nVertices > 0)
			{
				//pMeshInfo->m_nType |= VERTEXT_POSITION;
				XMFLOAT2* TexCoords = new XMFLOAT2[nVertices];
				int nReads = (UINT)::fread(TexCoords, sizeof(XMFLOAT2), nVertices, pInFile);
				
				delete[] TexCoords;

			}
		}
		else if (!strcmp(pstrToken, "<Indices>:"))
		{
			nIndices = ReadIntegerFromFile(pInFile);
			pMeshInfo->m_nIndices = nIndices;
			if (nIndices > 0)
			{
				pMeshInfo->m_pnIndices = new UINT[nIndices];
				nReads = (UINT)::fread(pMeshInfo->m_pnIndices, sizeof(UINT), nIndices, pInFile);

			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			pMeshInfo->m_nSubMeshes = ReadIntegerFromFile(pInFile);
			if (pMeshInfo->m_nSubMeshes > 0)
			{
				pMeshInfo->m_pnSubSetIndices = new int[pMeshInfo->m_nSubMeshes];
				pMeshInfo->m_ppnSubSetIndices = new UINT * [pMeshInfo->m_nSubMeshes];
				for (int i = 0; i < pMeshInfo->m_nSubMeshes; i++)
				{
					ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = ReadIntegerFromFile(pInFile);
						pMeshInfo->m_pnSubSetIndices[i] = ReadIntegerFromFile(pInFile);
						if (pMeshInfo->m_pnSubSetIndices[i] > 0)
						{
							pMeshInfo->m_ppnSubSetIndices[i] = new UINT[pMeshInfo->m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(pMeshInfo->m_ppnSubSetIndices[i], sizeof(int), pMeshInfo->m_pnSubSetIndices[i], pInFile);
						}

					}
				}
			}
		}
		else if (!strcmp(pstrToken, ""))
		{
			break;

		}
	}


	return(pMeshInfo);

}
