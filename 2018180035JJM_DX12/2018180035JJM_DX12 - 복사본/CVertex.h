#pragma once
class CVertex
{

private:
	XMFLOAT3 m_vPos;
	XMFLOAT4 m_color;
	XMFLOAT2 m_UV;


public:
	void SetPosition(XMFLOAT3 vPos) { m_vPos = vPos; }
	void SetPosition(float _x, float _y, float _z) { m_vPos = XMFLOAT3(_x, _y, _z); }

	void SetUv(XMFLOAT2 uv) { m_UV = uv; }


	void SetColor(XMFLOAT4 _f4DiffuseColor) { m_color = _f4DiffuseColor; }
	void SetColor(XMVECTORF32 _vecf32DiffuseColor) { m_color = XMVECTORF32toXMFLOAT4(_vecf32DiffuseColor); }


public:
	CVertex();
	CVertex(XMFLOAT3 vPos, XMFLOAT2 uv, XMFLOAT4 color = XMVECTORF32toXMFLOAT4(DirectX::Colors::Cyan));
	CVertex(XMFLOAT3 vPos);
	~CVertex();

};

