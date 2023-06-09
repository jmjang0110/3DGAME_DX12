#pragma once
class CHeightMapImage
{
private:
	BYTE *		m_pHeightMapPixels;	///높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 각 픽셀은 0~255의 값을 갖는다. 
	int			m_nWidth;			///높이 맵 이미지의 가로와 세로 크기이다. 
	int			m_nLength;
	XMFLOAT3	m_xmf3Scale;		///높이 맵 이미지를 실제로 몇 배 확대하여 사용할 것인가를 나타내는 스케일 벡터이다. 


public:
	/// [ GET ]
	float		GetHeight(float fx, float fz);///높이 맵 이미지에서 (x, z) 위치의 픽셀 값에 기반한 지형의 높이를 반환한다. 
	XMFLOAT3	GetHeightMapNormal(int x, int z);///높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다. 

	XMFLOAT3	GetScale()				{ return(m_xmf3Scale); }
	BYTE*		GetHeightMapPixels()	{ return(m_pHeightMapPixels); }
	int			GetHeightMapWidth()		{ return(m_nWidth); }
	int			GetHeightMapLength()	{ return(m_nLength); }


public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);
};
