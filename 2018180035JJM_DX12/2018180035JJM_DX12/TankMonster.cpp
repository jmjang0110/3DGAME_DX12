
#include "stdafx.h"
#include "TankMonster.h"

#include "CTimer.h"
#include "CTransform.h"
#include "CKeyManager.h"

#include "CMesh.h"
#include "CGameFramework.h"
#include "CScene.h"

#include "BulletScript.h"
#include "CGameObject.h"
#include "ResourceManager.h"
#include "PlayerScript.h"



TankMonsterScript::TankMonsterScript()
{
	m_eType = SCRIPT_TYPE::TANK_MONSTER;


}

TankMonsterScript::TankMonsterScript(CGameObject* pOwner)
{
	m_eType = SCRIPT_TYPE::TANK_MONSTER;


}

TankMonsterScript::~TankMonsterScript()
{
	//STL_VECTOR_DELETE(m_pBullets)

}

void TankMonsterScript::Init()
{
	m_pBody   = CGameFramework::GetInst()->GetCurScene()->GetObjByName("TankMonster");
	m_pTurret = CScript::GetOwner()->FindChild("TurretMonster");
	m_pGun    = CGameFramework::GetInst()->GetCurScene()->GetObjByName("GunMontser");




	for (UINT i = 0; i < m_iBulletMax; ++i) {

		CGameObject* pBullet = new CGameObject;
		pBullet->SetPipelineStateKeyName("Illuminate");

		pBullet->SetName("Bullet");
		pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("Hellfire_Missile_Instance"));
		pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));

		CTransform* pTransCom = new CTransform;
		pBullet->AddComponent(pTransCom);

		BulletScript* pScript = new BulletScript;
		pBullet->AddScript(pScript);
		pScript->Init();

		//pBullet->AddScript(pScript);

		// ** DEACTIVATE **
		pBullet->Deactivate();
		CGameFramework::GetInst()->GetCurScene()->PushBack_GameObject(pBullet);
		m_pBullets.push_back(pBullet);
	}

	m_pCurScene = CGameFramework::GetInst()->GetCurScene();


}

void TankMonsterScript::Update(float _fTimeElapsed)
{
	if (!m_pTargetObj)
		m_pTargetObj = CGameFramework::GetInst()->GetCurScene()->GetObjByName("TankObject");

	if(!m_pBody)
		m_pBody = CGameFramework::GetInst()->GetCurScene()->GetObjByName("TankMonster");
	if (!m_pTurret)
		m_pTurret = CScript::GetOwner()->FindChild("TurretMonster");
	if (!m_pGun)
		m_pGun = CGameFramework::GetInst()->GetCurScene()->GetObjByName("GunMontser");

	CTransform* pTargetTrans = (CTransform*)m_pTargetObj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	CTransform* pPlayerTrans = (CTransform*)GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);

	XMFLOAT3 vTargetFront = pTargetTrans->GetWorldDir(DIR_TYPE::FRONT);
	XMFLOAT3 vPlayerFront = pPlayerTrans->GetWorldDir(DIR_TYPE::FRONT);

	XMFLOAT3 vTargetPos = pTargetTrans->GetCurPosition();
	XMFLOAT3 vPlayerPOs = pPlayerTrans->GetCurPosition();

	XMFLOAT3 vEnddir = Vector3::Subtract(vTargetPos, vPlayerPOs);
	vEnddir = Vector3::Normalize(vEnddir);

	RotateToPlayer(vEnddir, vPlayerFront, _fTimeElapsed);

	if (GetOwner()->GetName() == "TankMonster")
	{
		UpdateMove(_fTimeElapsed);
		m_fTimer += _fTimeElapsed;
		if (m_fTimer >= 2.f)
		{
			m_fTimer = 0.f;
			Fire();
		}
	}


	if (m_pTargetObj) {
		PlayerScript* pScript = (PlayerScript*)m_pTargetObj->GetScript(SCRIPT_TYPE::PLAYER);
		bool CollideCheck = pScript->CollideCheck_Bullets(m_pBody);
		if (CollideCheck)
			m_iHitCnt++;
	}

	if (m_iHitCnt >= 30) {
		int check = MessageBox(nullptr, L"게임 종료",
			L"게임 종료", MB_ICONQUESTION | MB_OKCANCEL);
		if (check) {
			PostQuitMessage(0);
		}
	}
		
}

void TankMonsterScript::Start()
{
}

void TankMonsterScript::RotateToPlayer(XMFLOAT3 vEndDir, XMFLOAT3 vStartDir, float _fTimeElapsed)
{
	CTransform* pTargetTrans = (CTransform*)m_pTargetObj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	CTransform* pBodyTrans = (CTransform*)m_pBody->GetComponent(COMPONENT_TYPE::TRANSFORM);
	CTransform* pTrans = (CTransform*)GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);

	XMFLOAT3 pTargetRot = pTargetTrans->GetRotation();
	XMFLOAT3 vRot = pBodyTrans->GetRotation();
	vRot = pBodyTrans->ToRadian(vRot);

	XMFLOAT3 Edir = vEndDir;
	XMFLOAT3 Sdir = vStartDir;

	// 플레이어가 돌아야할 남은 각도 
	float	fRadAngle = GetRadAngle(Edir, Sdir);
	int		iRotDir = (fRadAngle <= 0.f) ? -1 : 1;

	if (iRotDir <= 0) { // CCW
		vRot.y += iRotDir * _fTimeElapsed * 1.f;

	}
	else {				// CW
		vRot.y += iRotDir * _fTimeElapsed * 1.f;

	}

	vRot = pTrans->ToDegree(vRot);
	pTrans->SetRotation(vRot);

	return;

	// 회전 시작 !
	if (m_iRotateDir_ToPastAxis == 0) {
		if (m_xmf3_PastTargetRot.y != fRadAngle)
			m_iRotateDir_ToPastAxis = iRotDir;
	}
	// 회전 끝 
	else if (m_iRotateDir_ToPastAxis != iRotDir) {
		m_iRotateDir_ToPastAxis = 0;
		m_xmf3_PastTargetRot = pTargetRot;

	}
	// 회전 중 
	else
	{
		if (m_xmf3_PastTargetRot.y != fRadAngle)
		{
			if (iRotDir <= 0) { // CCW
				vRot.y += iRotDir * _fTimeElapsed * 1.f;

			}
			else {				// CW
				vRot.y += iRotDir * _fTimeElapsed * 1.f;

			}
		}

	}
	vRot = pTrans->ToDegree(vRot);
	pTrans->SetRotation(vRot);

	m_iRotateDir_ToPastAxis = iRotDir;
}


void TankMonsterScript::UpdateMove(float _fTimeElapsed)
{
	CTransform* ptrans = (CTransform*)m_pBody->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 vCurPos = ptrans->GetCurPosition();

	m_fAcc += _fTimeElapsed * m_iMoveDir;
	if (m_fAcc >= 4)
		m_iMoveDir = -1;
	else if (m_fAcc <= -4)
		m_iMoveDir = 1;

	vCurPos.x += _fTimeElapsed * m_fMoveSpeed * m_iMoveDir;
	ptrans->SetPosition(vCurPos);

}

float TankMonsterScript::GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start)
{
	XMVECTOR _vA = XMLoadFloat3(&_Standard);
	XMVECTOR _vB = XMLoadFloat3(&_Start);

	if (XMVector3Equal(_vA, _vB))
		return 0.f;
	if (XMVector3Equal(_vA, -1 * _vB))
		return XM_PI;

	// [ -180 ~ 180 Degree ]

		// 카메라가		앞을 보는 벡터와
		// 플레이어가	앞을 보는 벡터 사이의 각을 갱신한다. 

	_vA = XMVector3Normalize(_vA);
	_vB = XMVector3Normalize(_vB);

	// 두 벡터 사이의 내적값은 0 ~ 180 도 사이 값이 나온다. -> 외적값을 이용해 CW/CCW 이동방향인지 확인한다. 
	XMVECTOR X = XMVector3Dot(_vA, _vB);
	float fCos = XMVectorGetX(X);
	if (fCos >= 1.f) fCos = 1.f;
	if (fCos <= -1.f) fCos = -1.f;

	float fRotRadian = acos(fCos);
	float fRotAngle = fRotRadian * (180.f / XM_PI);

	XMVECTOR vResult = XMVector3Cross(_vA, _vB);
	XMFLOAT3 vCross;
	XMStoreFloat3(&vCross, vResult);

	// _vTarget 기준으로 음수 방향으로 돌아야 하는 경우 ( 왼쪽 ) 
	if (vCross.y > 0.f)
	{
		//fRotRadian = acos(_vC.Dot(_vB)) + XM_PI;
		fRotRadian *= -1.f;
		fRotAngle *= -1.f;
	}

	return fRotRadian;
}

void TankMonsterScript::UpdatePosition(float _fTimeElapsed)
{

}

void TankMonsterScript::Fire()
{
	if (m_pGun == nullptr) {
		m_pGun = CGameFramework::GetInst()->GetCurScene()->GetObjByName("GunMontser");
		return;
	}

	CTransform* pTrans = (CTransform*)CScript::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 vCurPos = pTrans->GetCurPosition();
	CTransform* pGunTrans = (CTransform*)m_pGun->GetComponent(COMPONENT_TYPE::TRANSFORM);


	for (auto pBullet : m_pBullets) {
		if (pBullet->IsActive() == false)
		{
			CTransform* pBulletTrans = (CTransform*)pBullet->GetComponent(COMPONENT_TYPE::TRANSFORM);
			BulletScript* pScript = (BulletScript*)pBullet->GetScript(SCRIPT_TYPE::BULLET);

			XMFLOAT3 vGunRot = pGunTrans->GetRotation();

			XMFLOAT3 BulletStartPos = vCurPos;
			BulletStartPos.y += 5.f;
			XMFLOAT3 vFront = pGunTrans->GetWorldDir(DIR_TYPE::FRONT);
			BulletStartPos.x += vFront.x * 15.f;
			BulletStartPos.y += vFront.y * 15.f;
			BulletStartPos.z += vFront.z * 15.f;


			pBulletTrans->SetPosition(BulletStartPos);
			pBulletTrans->SetRotation(vGunRot);
			//pBulletTrans->SetScale(2.f, 2.f, 2.f);

			XMFLOAT3 vForwardDir = pGunTrans->GetWorldDir(DIR_TYPE::FRONT);

			pScript->SetDir(vForwardDir);
			pBullet->Activate();
			pScript->Start();

			break;
		}
	}


}

bool TankMonsterScript::CollideCheck_Bullets(CGameObject* pTarget)
{
	if (!pTarget) return false;

	CTransform* pTargetTrans = pTarget->Get_Transform_Component();
	XMFLOAT3	vTargetPos = pTargetTrans->GetCurPosition();
	bool Shot = false;

	for (auto Bullet : m_pBullets) {
		if (Bullet->IsActive() == false) continue;
		CTransform* pTrans = Bullet->Get_Transform_Component();
		XMFLOAT3	BulletPos = pTrans->GetCurPosition();


		bool CollideCheck = Bullet->CollideCheck_Sphere(vTargetPos);

		// 총 맞음 
		if (CollideCheck == true) {
			BulletScript* pScript = (BulletScript*)Bullet->GetScript(SCRIPT_TYPE::BULLET);
			pScript->Reset();
			Bullet->Deactivate();
			Shot = true;
		}
	}

	return Shot;
}
