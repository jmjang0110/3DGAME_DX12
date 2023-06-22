
#include "stdafx.h"

#include "AIScript.h"
#include "CTransform.h"
#include "CGameFramework.h"
#include "CGameObject.h"

#include "CScene.h"
#include "CHeightmapGrid.h"
#include "CHeightMapImage.h"
#include "CHeightmapTerrain.h"

#include "CFileManager.h"
#include "ResourceManager.h"
#include "BulletScript.h"


AIScript::AIScript()
{
}

AIScript::AIScript(CGameObject* pOwner)
{
	CScript::SetOWner(pOwner);

}

AIScript::~AIScript()
{
}

void AIScript::Init()
{
	m_pTurret = CScript::GetOwner()->FindChild("Turret");
	m_pGun = CScript::GetOwner()->FindChild("Gun");
	CGameObject* pObj = CFileManager::GetInst()->LoadGeometryFromFile("Model/Apache.bin");
	CGameObject* pBullet = CFileManager::GetInst()->LoadGeometryFromFile("Model/Gunship.bin");


	for (UINT i = 0; i < m_iBulletMax; ++i) {

		CGameObject* pBullet = new CGameObject;
		pBullet->SetPipelineStateKeyName("Illuminate");

		pBullet->SetName("Bullet");
		pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("Cube"));
		pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));

		CTransform* pTransCom = new CTransform;
		pTransCom->SetScale(0.05f, 0.05f, 0.05f);
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

	SAFE_DELETE(pObj);
	SAFE_DELETE(pBullet);
}

void AIScript::Update(float _fTimeElapsed)
{
	if (!m_pCurScene)
	{
		m_pCurScene = CGameFramework::GetInst()->GetCurScene();
	}

	if (m_pTarget == nullptr)
	{
		m_pTarget = CGameFramework::GetInst()->GetCurScene()->GetObjByName("Player");
	}


	CTransform* pTargetTrans = m_pTarget->Get_Transform_Component();
	XMFLOAT3	TargetPos    = pTargetTrans->GetCurPosition();

	CTransform* pMyTrans = CScript::GetOwner()->Get_Transform_Component();
	XMFLOAT3	MyPos    = pMyTrans->GetCurPosition();

	if (m_eMoveState == AI_MOVE_STATE::START)
	{
		// 하이트 맵 위에 랜덤한 위치를 부여받는다. ( x, z )
		int Width = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapWidth();
		int Length = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapLength();
		XMFLOAT3 scale = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetScale();

		m_vMove2Pos.x = rand() % (int)(Width * scale.x);
		m_vMove2Pos.z = rand() % (int)(Length * scale.z);
		m_bMoveFinish = false;
		m_eMoveState = AI_MOVE_STATE::ING;

	}
	else if (m_eMoveState == AI_MOVE_STATE::ING)
	{
		XMFLOAT3 MoveDir = XMFLOAT3{};
		MoveDir.y = 0.f;
		MoveDir.x = m_vMove2Pos.x - MyPos.x;
		MoveDir.z = m_vMove2Pos.z - MyPos.z;
		MoveDir = Vector3::Normalize(MoveDir);

		XMFLOAT3 lhs = m_vMove2Pos;
		lhs.y = 0.f;
		XMFLOAT3 rhs = MyPos;
		rhs.y = 0.f;

		float Dist = Vector3::Distance(lhs, rhs);

		MyPos.x += MoveDir.x * _fTimeElapsed * 20.f;
		MyPos.z += MoveDir.z * _fTimeElapsed * 20.f;

		if (Dist <= 15.f)
			m_eMoveState = AI_MOVE_STATE::FINISH;
	}
	else if (m_eMoveState == AI_MOVE_STATE::FINISH)
	{
		m_bMoveFinish = true;
		m_eMoveState = AI_MOVE_STATE::START;
	}


	/// 인식범위 안에 들어왔다면  
	float fDist = Vector3::Distance(TargetPos, MyPos);
	if (fDist <= m_fRecogDist)
	{
		m_FireTimer += _fTimeElapsed;
		if (m_FireTimer >= m_FireMaxTime)
		{
			Fire();
			m_FireTimer = 0.f;
		}
	}



	/// Terrain Height map 올린다. 
	CTransform* pTrans = GetOwner()->Get_Transform_Component();
	float fHeight      = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeight(MyPos.x, MyPos.z);
	MyPos.y             = fHeight;

	pTrans->SetPosition(MyPos);

}

void AIScript::Fire()
{
	CTransform* pTargetTrans = m_pTarget->Get_Transform_Component();
	XMFLOAT3	TargetPos = pTargetTrans->GetCurPosition();

	CTransform* pMyTrans = CScript::GetOwner()->Get_Transform_Component();
	XMFLOAT3	MyPos    = pMyTrans->GetCurPosition();


	for (auto pBullet : m_pBullets) {
		if (pBullet->IsActive() == false)
		{
			XMFLOAT3 vFireDir = Vector3::Subtract(TargetPos, MyPos);
			vFireDir = Vector3::Normalize(vFireDir);


			//pBullet->DenyRenderDirect();
			CTransform* pBulletTrans = (CTransform*)pBullet->GetComponent(COMPONENT_TYPE::TRANSFORM);
			BulletScript* pScript = (BulletScript*)pBullet->GetScript(SCRIPT_TYPE::BULLET);

			CTransform* pGunTrans = (CTransform*)m_pGun->GetComponent(COMPONENT_TYPE::TRANSFORM);

			XMFLOAT3 vGunRot = pGunTrans->GetRotation();
			vGunRot = pGunTrans->ToRadian(vGunRot);

			XMFLOAT3 Axis1 = XMFLOAT3(0.f, 0.f, 1.f);
			XMFLOAT3 Axis2 = XMFLOAT3(1.f, 0.f, 0.f);


			XMFLOAT3 Axis1_1 = XMFLOAT3(vFireDir.x, 0.f, vFireDir.z);
			XMFLOAT3 Axis2_1 = XMFLOAT3(0.f, vFireDir.y, vFireDir.z);
			float f = Vector3::Angle(Axis1, Axis2_1); // radian angle
			float f2 = f * 180 / XM_PI;

			//float f2 = Vector3::Angle(Axis1_1, Axis1); // radian angle
			//vGunRot.y = f;

			vGunRot.y = GetRadAngle(Axis1_1, Axis1);
			//vGunRot.y *= -1;
			//vGunRot.x = GetRadAngle(Axis2_1, Axis1);
			//vGunRot.x *= iRotDir;
			vGunRot.z = 0.f;

			vGunRot = pGunTrans->ToDegree(vGunRot);
			pGunTrans->SetRotation(vGunRot);

			XMFLOAT3 BulletStartPos = MyPos;
			BulletStartPos.y += 5.f;
			XMFLOAT3 vFront = pGunTrans->GetWorldDir(DIR_TYPE::FRONT);
			BulletStartPos.x += vFront.x * 15.f;
			BulletStartPos.y += vFront.y * 15.f;
			BulletStartPos.z += vFront.z * 15.f;


			pBulletTrans->SetPosition(BulletStartPos);
			pBulletTrans->SetRotation(vGunRot);
			//pBulletTrans->SetScale(2.f, 2.f, 2.f);


			XMFLOAT3 vForwardDir = pGunTrans->GetWorldDir(DIR_TYPE::FRONT);

			pScript->SetDir(vFireDir);
			pBullet->Activate();
			pScript->Start();

			break;
		}
	}

}

float AIScript::GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start)
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

bool AIScript::CollideCheck_Bullets(CGameObject* pTarget)
{
	return false;
}
