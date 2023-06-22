
#include "stdafx.h"
#include "CScript.h"

#include "RotateScript.h"
#include "CGameFramework.h"
#include "CGameObject.h"
#include "CTransform.h"



RotateScript::RotateScript() : CScript()

{
}

RotateScript::RotateScript(CGameObject* pOwner) : CScript()
{
	CScript::SetOWner(pOwner);
	m_pTarget = pOwner;


}

RotateScript::~RotateScript()
{
}

void RotateScript::Init()
{

}

void RotateScript::Update(float _fTimeElapsed)
{

	if (m_pTarget == nullptr)
		return;

	CTransform* pTrans = m_pTarget->Get_Transform_Component();
	XMFLOAT3	vRot   = pTrans->GetRotation();

	vRot = pTrans->ToRadian(vRot);
	vRot.y += _fTimeElapsed * m_fSpeed * XM_PI;
	vRot = pTrans->ToDegree(vRot);

	pTrans->SetRotation(vRot);

}
