#pragma once
class CVertex
{

private:
	XMFLOAT3 m_vPos;
	XMFLOAT4 m_vDiffuseColor = XMVECTORF32toXMFLOAT4(Colors::Cyan); // 정점의 색상 

public:
	void SetPosition(XMFLOAT3 vPos) { m_vPos = vPos; }
	void SetPosition(float _x, float _y, float _z) { m_vPos = XMFLOAT3(_x, _y, _z); }

	void SetDiffuseColor(XMFLOAT4 _f4DiffuseColor) { m_vDiffuseColor = _f4DiffuseColor; }
	void SetDiffuseColor(XMVECTORF32 _vecf32DiffuseColor) { m_vDiffuseColor = XMVECTORF32toXMFLOAT4(_vecf32DiffuseColor); }


public:
	CVertex();
	CVertex(XMFLOAT3 vPos);
	~CVertex();

};

