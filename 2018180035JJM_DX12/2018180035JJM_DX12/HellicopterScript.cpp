#include "stdafx.h"
#include "HellicopterScript.h"
#include "CGameObject.h"

#include "CScript.h"
#include "CTimer.h"
#include "CTransform.h"
#include "CKeyManager.h"

#include "CMesh.h"
#include "CGameFramework.h"
#include "CScene.h"

#include "CCamera.h"
#include "CFileManager.h"
#include "ResourceManager.h"
#include "BulletScript.h"

#include "CHeightmapGrid.h"
#include  "CHeightMapImage.h"
#include "CHeightmapTerrain.h"



void HellicopterScript::UpdateAimPoint()
{
	if (m_pAimPoint == nullptr)
		return;

	CTransform* pAimTrans = m_pAimPoint->Get_Transform_Component();
	XMFLOAT3 vAimPos      = pAimTrans->GetCurPosition();

	CTransform* pTrans   = CScript::GetOwner()->Get_Transform_Component();
	XMFLOAT3 vfrontDir   = pTrans->GetWorldDir(DIR_TYPE::FRONT);
	XMFLOAT3 vRot		 = pTrans->GetRotation();

	vAimPos = pTrans->GetCurPosition();

	float Dist = 70.f;
	vAimPos.x += vfrontDir.x * Dist;
	vAimPos.y += vfrontDir.y * Dist;
	vAimPos.z += vfrontDir.z * Dist;


	pAimTrans->SetPosition(vAimPos);
	pAimTrans->SetRotation(vRot);
	pAimTrans->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));


}

HellicopterScript::HellicopterScript()
{

}

HellicopterScript::HellicopterScript(CGameObject* pOwner) :
	CScript(pOwner)
{

}

HellicopterScript::~HellicopterScript()
{

}

void HellicopterScript::Init()
{

	CGameObject* pBullet = CFileManager::GetInst()->LoadGeometryFromFile("Model/Gunship.bin");


	for (UINT i = 0; i < m_iBulletMax; ++i) {

		CGameObject* pBullet = new CGameObject;
		pBullet->SetPipelineStateKeyName("Illuminate");

		pBullet->SetName("Bullet");
		pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("Hellfire_Missile_Instance"));
		pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));

		CTransform* pTransCom = new CTransform;
		pBullet->AddComponent(pTransCom);

		BulletScript* pScript = new BulletScript;
		pScript->SetSpeed(150.f);
		pBullet->AddScript(pScript);
		pScript->Init();

		//pBullet->AddScript(pScript);

		// ** DEACTIVATE **
		pBullet->Deactivate();
		CGameFramework::GetInst()->GetCurScene()->PushBack_GameObject(pBullet);
		m_pBullets.push_back(pBullet);
	}

	m_pCurScene = CGameFramework::GetInst()->GetCurScene();
	SAFE_DELETE(pBullet);

}

void HellicopterScript::Update(float _fTimeElapsed)
{
	if (!m_pCurScene)
		m_pCurScene = CGameFramework::GetInst()->GetCurScene();

	Update_InputKey(_fTimeElapsed);

	Rotate_PlayerAxis_Sync_Camera_Axis(_fTimeElapsed);

	UpdateAimPoint();

	CTransform* pTrans = GetOwner()->Get_Transform_Component();
	XMFLOAT3 vPos = pTrans->GetCurPosition();
	float fHeight = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeight(vPos.x, vPos.z);
	
	if (vPos.y < fHeight)
	{
		vPos.y = fHeight;
		pTrans->SetPosition(vPos);

	/*	int check = MessageBox(nullptr, L"[  땅과 충돌! ] 패배!",
			L"게임 종료", MB_ICONQUESTION | MB_OKCANCEL);
		if (check) {
			PostQuitMessage(0);
		}*/
	}

	std::vector<CGameObject*> vecMonsters = CGameFramework::GetInst()->GetCurScene()->GetMonsters();
	for (auto Monster : vecMonsters)
	{
		if (Monster->IsActive())
		{
			bool bShot = CollideCheck_Bullets(Monster);
			if (bShot == true)
			{
				m_iKillMonsterCount++;
				Monster->Deactivate();
			}
		}

	}

	if (m_iKillMonsterCount >= 20)
	{
		int check = MessageBox(nullptr, L"[  적 처리 완료! ] 승리!",
			L"게임 종료", MB_ICONQUESTION | MB_OKCANCEL);
		if (check) {
			PostQuitMessage(0);
		}
	}
	//pTrans->SetPosition(vPos);

}

void HellicopterScript::UpdateCamera_ThirdPersonMode(float _fTimeElapsed)
{

	if (!m_pMainCam)
		m_pMainCam = MAIN_CAMERA(CGameFramework);

	CTransform* pCamTrans = m_pMainCam->Get_Transform_Component();
	XMFLOAT3	vRot = pCamTrans->GetRotation();
	const XMFLOAT2 vMouseDir = CKeyManager::GetInst()->GetMouseDir();


	if (KEY_PRESSED(m_eCameraMoveKey))
	{
		vRot = pCamTrans->ToRadian(vRot);

		vRot.y += _fTimeElapsed * vMouseDir.x * XM_PI;
		vRot.x -= _fTimeElapsed * vMouseDir.y * XM_PI;

		vRot = pCamTrans->ToDegree(vRot);

		pCamTrans->SetRotation(vRot);
	}


	// CAMERA 
	CTransform* pCamTransform = m_pMainCam->Get_Transform_Component();
	CCamera* pCamCamera = m_pMainCam->Get_Camera_Component();
	XMFLOAT3	xmf3Campos = pCamTransform->GetCurPosition();

	// TARGET
	CGameObject* pTargetObj = CScript::GetOwner(); // Player
	CTransform* pTargetTrans = (CTransform*)pTargetObj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	AXIS		 tTragetObjAxis = pTargetTrans->GetAxis();
	XMFLOAT3	 vTargetpos = pTargetTrans->GetCurPosition();


	// CAMERA MOVE 
	XMFLOAT3 xmf3Direction = Vector3::Subtract(vTargetpos, xmf3Campos);
	float fLength = Vector3::Length(xmf3Direction);

	XMFLOAT3 vBackWard = pCamTrans->GetWorldDir(DIR_TYPE::BACKWARD);

	xmf3Campos.x = vTargetpos.x + (vBackWard.x * m_fMaxOffsetDistance);
	xmf3Campos.y = m_fOffsetHeight + vTargetpos.y + (vBackWard.y * m_fMaxOffsetDistance);
	xmf3Campos.z = vTargetpos.z + (vBackWard.z * m_fMaxOffsetDistance);

	XMFLOAT3 vUp = pTargetTrans->GetWorldDir(DIR_TYPE::UP);


	//CAMERA LOOK AT / POSITION SETTING 
	pCamCamera->SetLookAt(xmf3Campos, vTargetpos, vUp);
	pCamTransform->SetPosition(xmf3Campos);

}

void HellicopterScript::Rotate_PlayerAxis_Sync_Camera_Axis(float _fTimeElapsed)
{
	if (!m_pMainCam) return;

	CTransform* pCamTrans = m_pMainCam->Get_Transform_Component();
	CTransform* pPlayerTrans = GetOwner()->Get_Transform_Component();

	XMFLOAT3 vCamFront = pCamTrans->GetWorldDir(DIR_TYPE::FRONT);
	XMFLOAT3 vPlayerFront = pPlayerTrans->GetWorldDir(DIR_TYPE::FRONT);

	RotatePlayer(vCamFront, vPlayerFront, _fTimeElapsed);
}
float HellicopterScript::GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start) {

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

	double fRotRadian = acos(fCos);
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


void HellicopterScript::RotatePlayer(XMFLOAT3 vEndDir, XMFLOAT3 vStartDir, float _fTimeElapsed)
{
	CTransform* pCamTrans = m_pMainCam->Get_Transform_Component();
	XMFLOAT3 vCamRot = pCamTrans->GetRotation();

	CTransform* pTrans = GetOwner()->Get_Transform_Component();
	XMFLOAT3 vRot = pTrans->GetRotation();

	vRot.x = vCamRot.x;

	vRot = pTrans->ToRadian(vRot);

	XMFLOAT3 Edir = vEndDir;
	XMFLOAT3 Sdir = vStartDir;

	// 플레이어가 돌아야할 남은 각도 
	float	fRadAngle = GetRadAngle(Edir, Sdir);
	int		iRotDir = (fRadAngle <= 0.f) ? -1 : 1;

	// 회전 시작 !
	if (m_iRotateDir_Past_ForCamAxis == 0) {
		if (m_xmf3_PastCamRot.y != vCamRot.y)
			m_iRotateDir_Past_ForCamAxis = iRotDir;
	}
	// 회전 끝 
	else if (m_iRotateDir_Past_ForCamAxis != iRotDir) {
		m_iRotateDir_Past_ForCamAxis = 0;
		m_xmf3_PastCamRot = vCamRot;

	}
	// 회전 중 
	else
	{
		if (m_xmf3_PastCamRot.y != vCamRot.y)
		{
			if (iRotDir <= 0) { // CCW
				vRot.y += iRotDir * _fTimeElapsed * m_fRotSpeed;

			}
			else {				// CW
				vRot.y += iRotDir * _fTimeElapsed * m_fRotSpeed;
			}
		}

	}


	vRot = pTrans->ToDegree(vRot);
	pTrans->SetRotation(vRot);

	m_iRotateDir_Past_ForCamAxis = iRotDir;
}

bool HellicopterScript::CheckRotateFinish(float _fRot_DT)
{
	float fDT = (_fRot_DT > 0.f) ? -1 * _fRot_DT : 1 * _fRot_DT;
	m_fMoveAccRot += fDT;
	if (m_fMoveAccRot <= 0.f)
	{
		m_fMoveAccRot = 0.f;
		return true;
	}
	else
		return false;
}


void HellicopterScript::Update_InputKey(float _fTimeElapsed)
{
	if (!m_pCurScene) return;

	if (!m_pMainCam)
		m_pMainCam = m_pCurScene->GetMainCamera();

	CTransform* pTrans = GetOwner()->Get_Transform_Component();
	XMFLOAT3 vCurPos = pTrans->GetCurPosition();
	CTransform* pCamTrans = m_pMainCam->Get_Transform_Component();

	UpdateCamera_ThirdPersonMode(_fTimeElapsed);

	if (KEY_PRESSED(m_eFrontKey)) {
		XMFLOAT3 vDir = pCamTrans->GetWorldDir(DIR_TYPE::FRONT);

		vCurPos.x += _fTimeElapsed * m_fMoveSpeed * vDir.x;
		vCurPos.y += _fTimeElapsed * m_fMoveSpeed * vDir.y;
		vCurPos.z += _fTimeElapsed * m_fMoveSpeed * vDir.z;


	}
	if (KEY_PRESSED(m_eBackKey)) {
		XMFLOAT3 vDir = pCamTrans->GetWorldDir(DIR_TYPE::BACKWARD);


		vCurPos.x += _fTimeElapsed * m_fMoveSpeed * vDir.x;
		vCurPos.y += _fTimeElapsed * m_fMoveSpeed * vDir.y;
		vCurPos.z += _fTimeElapsed * m_fMoveSpeed * vDir.z;
	}

	if (KEY_PRESSED(m_eLeftKey)) {
		XMFLOAT3 vDir = pCamTrans->GetWorldDir(DIR_TYPE::LEFT);


		vCurPos.x += _fTimeElapsed * m_fMoveSpeed * vDir.x;
		vCurPos.y += _fTimeElapsed * m_fMoveSpeed * vDir.y;
		vCurPos.z += _fTimeElapsed * m_fMoveSpeed * vDir.z;
	}

	if (KEY_PRESSED(m_eRightKey)) {
		XMFLOAT3 vDir = pCamTrans->GetWorldDir(DIR_TYPE::RIGHT);


		vCurPos.x += _fTimeElapsed * m_fMoveSpeed * vDir.x;
		vCurPos.y += _fTimeElapsed * m_fMoveSpeed * vDir.y;
		vCurPos.z += _fTimeElapsed * m_fMoveSpeed * vDir.z;
	}


	pTrans->SetPosition(vCurPos);


	if (KEY_AWAY(m_eAttackKey)) {
		for (auto pBullet : m_pBullets) {
			if (pBullet->IsActive() == false)
			{
				//pBullet->DenyRenderDirect();
				CTransform* pBulletTrans = (CTransform*)pBullet->GetComponent(COMPONENT_TYPE::TRANSFORM);
				BulletScript* pScript = (BulletScript*)pBullet->GetScript(SCRIPT_TYPE::BULLET);

				CTransform* pGunTrans = (CTransform*)m_pAimPoint->GetComponent(COMPONENT_TYPE::TRANSFORM);
				XMFLOAT3 vGunRot = pGunTrans->GetRotation();

				XMFLOAT3 BulletStartPos = vCurPos;
				BulletStartPos.y += 0.f;
				XMFLOAT3 vFront = pGunTrans->GetWorldDir(DIR_TYPE::FRONT);
				BulletStartPos.x += vFront.x * 3.f;
				BulletStartPos.y += vFront.y * 1.f;
				BulletStartPos.z += vFront.z * 3.f;


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


	if (KEY_PRESSED(KEY::NUM0))
	{
		CTransform* pTrans = GetOwner()->Get_Transform_Component();
		XMFLOAT3 Pos = pTrans->GetCurPosition();
		Pos.y += _fTimeElapsed * 50.f;
		pTrans->SetPosition(Pos);

	}
	if (KEY_PRESSED(KEY::NUM1))
	{
		CTransform* pTrans = GetOwner()->Get_Transform_Component();
		XMFLOAT3 Pos = pTrans->GetCurPosition();
		Pos.y -= _fTimeElapsed * 50.f;
		pTrans->SetPosition(Pos);

	}






}

bool HellicopterScript::CollideCheck_Bullets(CGameObject* pTarget)
{
	if (!pTarget) return false;

	CTransform* pTargetTrans = pTarget->Get_Transform_Component();
	XMFLOAT3	vTargetPos   = pTargetTrans->GetCurPosition();
	bool Shot                = false;

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

