#include "stdafx.h"
#include "CVertex.h"

CVertex::CVertex()
{
	m_vPos			= XMFLOAT3(0.f, 0.f, 0.f);
	m_color			= ::XMVECTORF32toXMFLOAT4(Colors::Cyan);

}

CVertex::CVertex(XMFLOAT3 vPos, XMFLOAT2 uv, XMFLOAT4 color)
{
	m_vPos  = vPos;
	m_color = color;
	m_UV    = uv;

}

CVertex::CVertex(XMFLOAT3 vPos)
{
	m_vPos = vPos;
}

CVertex::~CVertex()
{
}
