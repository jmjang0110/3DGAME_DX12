
#include "stdafx.h"
#include "CameraScript.h"

#include "CGameFramework.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CComponent.h"
#include "CScene.h"


#include "CKeyManager.h"



CameraScript::CameraScript()
{
}

CameraScript::CameraScript(CGameObject* pOwner)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::Init()
{

}

void CameraScript::Update(float _fTimeElapsed)
{
	CGameObject* pCam		= MAIN_CAMERA(CGameFramework);
	CTransform* pCamTrans	= pCam->Get_Transform_Component();
	CCamera*	pCamCam		= pCam->Get_Camera_Component();
	XMFLOAT3	vRot		= pCamTrans->GetRotation();
	XMFLOAT3	vPos		= pCamTrans->GetCurPosition();

	const XMFLOAT2 vMouseDir = CKeyManager::GetInst()->GetMouseDir();
	if (KEY_PRESSED(KEY::RBTN))
	{
		vRot = pCamTrans->ToRadian(vRot);

		vRot.y += _fTimeElapsed * vMouseDir.x * XM_PI;
		vRot.x -= _fTimeElapsed * vMouseDir.y * XM_PI;

		vRot = pCamTrans->ToDegree(vRot);

		pCamTrans->SetRotation(vRot);
	}

	XMFLOAT3 vForDir = pCamTrans->GetWorldDir(DIR_TYPE::FRONT);
	XMFLOAT3 vUpDir = pCamTrans->GetWorldDir(DIR_TYPE::UP);

	XMFLOAT3 vLookAt = XMFLOAT3(vForDir.x * 1000.f, vForDir.y * 1000.f, vForDir.z * 1000.f);

	pCamCam->SetLookAt(vPos, vLookAt, vUpDir);

	
	float fMoveSpeed = 200;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.z += _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}
	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}
	if (KEY_PRESSED(KEY::S))
	{
		vPos.z -= _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}
	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}

	if (KEY_PRESSED(KEY::E))
	{
		vPos.y	+= _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}

	if (KEY_PRESSED(KEY::Q))
	{
		vPos.y -= _fTimeElapsed * fMoveSpeed;
		pCamTrans->SetPosition(vPos);
	}


	return;

}
