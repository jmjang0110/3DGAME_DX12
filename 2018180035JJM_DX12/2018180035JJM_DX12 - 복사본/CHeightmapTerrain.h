#pragma once

#include "CHeightMapImage.h"
class CGameObject;

class CHeightmapTerrain
{
private:
	
	CHeightMapImage*	m_pHeightMapImage; //지형의 높이 맵으로 사용할 이미지이다.
	
	int					m_nWidth;	//높이 맵 가로 크기
	int					m_nLength; // 높이 맵 세로 크기
	XMFLOAT3			m_xmf3Scale;	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.

	
	CGameObject* m_pTerrainObj;


public:
	CHeightmapTerrain(LPCTSTR pFileName
		, int nWidth
		, int nLength
		, int nBlockWidth
		, int nBlockLength
		, XMFLOAT3 xmf3Scale
		, XMFLOAT4 xmf4Color);

	virtual ~CHeightmapTerrain();



public:
	//지형의 높이를 계산하는 함수이다(월드 좌표계). 
	//높이 맵의 높이에 스케일의 y를 곱한 값이다. 
	float			GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x /m_xmf3Scale.x, z / m_xmf3Scale.z)* m_xmf3Scale.y);
}
//지형의 법선 벡터를 계산하는 함수이다(월드 좌표계). 높이 맵의 법선 벡터를 사용한다. 
	XMFLOAT3		GetNormal(float x, float z) {  return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z /m_xmf3Scale.z))); }
	int				GetHeightMapWidth()			{ return(m_pHeightMapImage->GetHeightMapWidth()); }
	int				GetHeightMapLength()		{ return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3		GetScale()					{ return(m_xmf3Scale); }
	float			GetWidth()					{ return(m_nWidth * m_xmf3Scale.x); }
	float			GetLength()					{ return(m_nLength * m_xmf3Scale.z); }

	CGameObject*	GetTerrainObject()			{ return m_pTerrainObj; }

};

