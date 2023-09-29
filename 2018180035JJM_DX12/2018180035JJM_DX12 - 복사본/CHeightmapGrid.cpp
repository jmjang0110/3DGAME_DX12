
#include "stdafx.h"
#include "CHeightmapGrid.h"
#include "CMesh.h"
#include "CHeightMapImage.h"

CHeightmapGrid::CHeightmapGrid()
{
}

CHeightmapGrid::~CHeightmapGrid()
{

}

float CHeightmapGrid::OnGetHeight(int x, int z, void* pContext)
{

	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	BYTE* pHeightMapPixels           = pHeightMapImage->GetHeightMapPixels();
	XMFLOAT3 xmf3Scale               = pHeightMapImage->GetScale();
	int nWidth                       = pHeightMapImage->GetHeightMapWidth();

	float fHeight = pHeightMapPixels[x + (z * nWidth)] * xmf3Scale.y;
	return(fHeight);

}

XMFLOAT4 CHeightmapGrid::OnGetColor(int x, int z, void* pContext)
{


	//조명의 방향 벡터(정점에서 조명까지의 벡터)이다. 
	XMFLOAT4 xmf4IncidentLightColor(0.9f, 0.8f, 0.4f, 1.0f);	//조명의 색상(세기, 밝기)이다. 
	XMFLOAT3 xmf3LightDirection      = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	xmf3LightDirection               = Vector3::Normalize(xmf3LightDirection);

	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	XMFLOAT3 xmf3Scale               = pHeightMapImage->GetScale();

	XMFLOAT3 Normal = pHeightMapImage->GetHeightMapNormal(x, z);
	float fScale = Vector3::DotProduct(Normal,xmf3LightDirection);
	Normal = pHeightMapImage->GetHeightMapNormal(x + 1, z);
	fScale      += Vector3::DotProduct(Normal ,xmf3LightDirection);

	Normal = pHeightMapImage->GetHeightMapNormal(x + 1, z + 1);
	fScale      += Vector3::DotProduct(Normal,xmf3LightDirection);

	Normal = pHeightMapImage->GetHeightMapNormal(x, z + 1);
	fScale      += Vector3::DotProduct(Normal,xmf3LightDirection);
	fScale       = (fScale / 4.0f) + 0.05f;
	
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;

	//fScale은 조명 색상(밝기)이 반사되는 비율이다. 
	XMFLOAT4 xmf4Color = Vector4::Multiply(fScale, xmf4IncidentLightColor);
	return(xmf4Color);

}