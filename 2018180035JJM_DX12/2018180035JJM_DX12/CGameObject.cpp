#include "stdafx.h"
#include "CGameObject.h"
#include "CGameFramework.h"

#include "CCommandQueue.h"
#include "CRootSignature.h"
#include "CGraphicsPipelineState.h"

#include "CMesh.h"
#include "CEntity.h"
#include "CComponent.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CScript.h"

#include "ResourceManager.h"


CGameObject::CGameObject() : CEntity()
{

}

CGameObject::~CGameObject()
{
	if (GetName() == "Gun")
		int i = 0;

	for (auto mesh : m_pMesh)
		mesh.reset();



	auto asdfasdf = ResourceManager::GetInst();

	for (auto iter : m_pMtrl)
		iter.reset();

	STL_VECTOR_DELETE(m_vecChild)
	STL_VECTOR_DELETE(m_vecSibling)
	STL_VECTOR_DELETE(m_vecScript)

	for (int i = 0; i < static_cast<UINT>(COMPONENT_TYPE::END); ++i)
		delete m_arrCom[i];


}

void CGameObject::Render()
{

	for (auto com : m_arrCom) {
		if(com) com->UpdateShaderVariables();
	}

	for (int i = 0; i < m_pMtrl.size(); ++i) {
		m_pMtrl[i]->UpdateShaderVariable();
		
		for (auto mesh : m_pMesh)
		{
			mesh->Render(i);
		}
	}

	for (auto pObj : m_vecChild) {
		pObj->Render();
	}


}

void CGameObject::Update(float _fTimeElapsed)
{
	for (auto com : m_arrCom) {
		if (com)  com->Update(_fTimeElapsed);
	}

	for (auto Scr : m_vecScript) {
		if (Scr) Scr->Update(_fTimeElapsed);
	}

	for(auto pObj : m_vecChild){
		pObj->Update(_fTimeElapsed);
	}

}

void CGameObject::FinalUpdate(float _fTimeElapsed)
{
	for (auto com : m_arrCom) {
		if (com)  com->FinalUpdate(_fTimeElapsed);
	}

	for (auto pObj : m_vecChild) {
		pObj->FinalUpdate(_fTimeElapsed);
	}

}

void CGameObject::ReleaseMeshUploadBuffers()
{
	for (auto pMesh : m_pMesh)
		pMesh->ReleaseUploadBuffers();

}

void CGameObject::Deactivate()
{
	m_bActive = false;

	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i) {
		m_arrCom[i]->Deactivate();
	}

	for (auto pChild : m_vecChild) {
		pChild->Deactivate();
	}

	for (const auto pScript : m_vecChild) {
		pScript->Deactivate();
	}


}

void CGameObject::Activate()
{
	m_bActive = true;
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i) {
		m_arrCom[i]->Activate();
	}

	for (const auto pScript : m_vecChild) {
		pScript->Activate();
	}

}

CScript* CGameObject::GetScript(SCRIPT_TYPE _eType)
{
	for (auto pScript : m_vecScript) {
		if (pScript->GetType() == _eType)
			return pScript;

	}
	return nullptr;
}


void CGameObject::SetMaterial(int nMaterial, std::shared_ptr<CMaterial> pMaterial)
{
	if (nMaterial - 1 <= m_pMtrl.size()) {
		m_pMtrl[nMaterial] = pMaterial;
	}
	else {
		m_pMtrl.push_back(pMaterial);
	}

}


void CGameObject::AddScript(CScript* pSCript)
{
	pSCript->SetOWner(this);
	m_vecScript.push_back(pSCript);

}

void CGameObject::AddComponent(CComponent* pCom)
{
	pCom->SetOwner(this);
	COMPONENT_TYPE comType = pCom->GetComponentType();
	m_arrCom[(int)comType] = pCom;
}

void CGameObject::AddChild(CGameObject* pObj)
{
	if (pObj == nullptr)
		return;

	pObj->m_pParent = this;
	m_vecChild.push_back(pObj);

}

CGameObject* CGameObject::FindChild(std::string _ChildName)
{
	CGameObject* pReturnObj = nullptr;
	for (const auto pChild : m_vecChild) {
		if (pChild->GetName() == _ChildName)
		{
			pReturnObj = pChild;
			break;
		}
		else
		{
			pReturnObj = pChild->FindChild(_ChildName);
			if (pReturnObj != nullptr)
				break;
		}
	}
	return pReturnObj;
}



bool CGameObject::MyCollideCheck(CGameObject* pObj)
{
	CTransform* pOtherTrans = (CTransform*)pObj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 vOtherPos = pOtherTrans->GetCurPosition();

	CTransform* pMyTrans = (CTransform*)this->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 vMyPos = pMyTrans->GetCurPosition();

	XMFLOAT3 vDist = Vector3::Subtract(vOtherPos, vMyPos);
	float fDist = Vector3::Length(vDist);

	if (fDist <= m_fCollideSphere_Radius)
		return true;
	else
		return false;

}

bool CGameObject::CollideCheck_Sphere(XMFLOAT3 vTargetPos)
{
	CTransform* pTrans = this->Get_Transform_Component();
	XMFLOAT3 vMyPos = pTrans->GetCurPosition();

	XMFLOAT3 vLength{};
	vLength.x = vTargetPos.x - vMyPos.x;
	vLength.y = vTargetPos.y - vMyPos.y;
	vLength.z = vTargetPos.z - vMyPos.z;

	float Distance = Vector3::Length(vLength);
	if (Distance <= m_fCollideSphere_Radius)
		return true;

	return false;
}
