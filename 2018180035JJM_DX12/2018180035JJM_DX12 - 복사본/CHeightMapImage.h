#pragma once
class CHeightMapImage
{
private:
	BYTE *		m_pHeightMapPixels;	///���� �� �̹��� �ȼ�(8-��Ʈ)���� ������ �迭�̴�. �� �ȼ��� 0~255�� ���� ���´�. 
	int			m_nWidth;			///���� �� �̹����� ���ο� ���� ũ���̴�. 
	int			m_nLength;
	XMFLOAT3	m_xmf3Scale;		///���� �� �̹����� ������ �� �� Ȯ���Ͽ� ����� ���ΰ��� ��Ÿ���� ������ �����̴�. 


public:
	/// [ GET ]
	float		GetHeight(float fx, float fz);///���� �� �̹������� (x, z) ��ġ�� �ȼ� ���� ����� ������ ���̸� ��ȯ�Ѵ�. 
	XMFLOAT3	GetHeightMapNormal(int x, int z);///���� �� �̹������� (x, z) ��ġ�� ���� ���͸� ��ȯ�Ѵ�. 

	XMFLOAT3	GetScale()				{ return(m_xmf3Scale); }
	BYTE*		GetHeightMapPixels()	{ return(m_pHeightMapPixels); }
	int			GetHeightMapWidth()		{ return(m_nWidth); }
	int			GetHeightMapLength()	{ return(m_nLength); }


public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);
};
