#pragma once

#include "CMesh.h"
#include "CMaterial.h"

class CGameObject;


class CFileManager
{
private:
	SINGLETON_PATTERN(CFileManager)

public:
	MATERIALSLOADINFO* LoadMaterialsInfoFromFile(FILE* pInFile);
	MeshLoadInfo* LoadMeshInfoFromFile(FILE* pInFile);
	std::vector<CGameObject*>  LoadSceneObjectsFromFile(char* pstrFileName);

	CGameObject* LoadFrameHierarchyFromFile(FILE* pInFile);
	CGameObject* LoadGeometryFromFile(std::string pstrFileName);

	MeshLoadInfo* LoadMeshInfo_Tank(std::string pstrFileName);


public:
	int ReadIntegerFromFile(FILE* pInFile);
	float ReadFloatFromFile(FILE* pInFile);
	BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);
	void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);

public:
	MeshLoadInfo* LoadFromFile_2(const char* pstrFileName);


};

