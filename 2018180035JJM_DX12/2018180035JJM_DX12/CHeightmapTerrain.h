#pragma once

#include "CHeightMapImage.h"
class CGameObject;

class CHeightmapTerrain
{
private:
	
	CHeightMapImage*	m_pHeightMapImage; //������ ���� ������ ����� �̹����̴�.
	
	int					m_nWidth;	//���� �� ���� ũ��
	int					m_nLength; // ���� �� ���� ũ��
	XMFLOAT3			m_xmf3Scale;	//������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ���� ������ �����̴�.

	
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
	//������ ���̸� ����ϴ� �Լ��̴�(���� ��ǥ��). 
	//���� ���� ���̿� �������� y�� ���� ���̴�. 
	float			GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x /m_xmf3Scale.x, z / m_xmf3Scale.z)* m_xmf3Scale.y);
}
//������ ���� ���͸� ����ϴ� �Լ��̴�(���� ��ǥ��). ���� ���� ���� ���͸� ����Ѵ�. 
	XMFLOAT3		GetNormal(float x, float z) {  return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z /m_xmf3Scale.z))); }
	int				GetHeightMapWidth()			{ return(m_pHeightMapImage->GetHeightMapWidth()); }
	int				GetHeightMapLength()		{ return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3		GetScale()					{ return(m_xmf3Scale); }
	float			GetWidth()					{ return(m_nWidth * m_xmf3Scale.x); }
	float			GetLength()					{ return(m_nLength * m_xmf3Scale.z); }

	CGameObject*	GetTerrainObject()			{ return m_pTerrainObj; }

};

