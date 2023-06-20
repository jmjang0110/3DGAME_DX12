#include "stdafx.h"
#include "CVertex.h"

CVertex::CVertex()
{
	m_vPos			= XMFLOAT3(0.f, 0.f, 0.f);
	m_vDiffuseColor = ::XMVECTORF32toXMFLOAT4(Colors::Cyan);

}

CVertex::CVertex(XMFLOAT3 vPos)
{
	m_vPos = vPos;
}

CVertex::~CVertex()
{
}
