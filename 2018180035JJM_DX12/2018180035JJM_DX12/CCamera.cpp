#include "stdafx.h"

#include "CCamera.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CCommandQueue.h"
#include "CGameFramework.h"
#include "ResourceManager.h"



CCamera::CCamera() : CComponent()
{
	CComponent::SetComponentType(COMPONENT_TYPE::CAMERA);


}

CCamera::~CCamera()
{
}

void CCamera::GenerateViewMatrix()
{
	CTransform* pCamTransform = (CTransform*)CComponent::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
	AXIS		tCurAxis = pCamTransform->GetAxis();
	XMFLOAT3	CurPos = pCamTransform->GetCurPosition();

	//  LOOK / RIGHT / UP VECTOR
	XMFLOAT3 xmf3Look = Vector3::Normalize(tCurAxis.xmf3Look);

	XMFLOAT3 Right = Vector3::CrossProduct(tCurAxis.xmf3Up, xmf3Look);
	XMFLOAT3 xmf3Right = Vector3::Normalize(Right);

	XMFLOAT3 UP = Vector3::CrossProduct(tCurAxis.xmf3Look, xmf3Right);
	XMFLOAT3 xmf3Up = Vector3::Normalize(UP);

	tCurAxis = AXIS(xmf3Right, xmf3Up, xmf3Look);
	pCamTransform->SetAxis(tCurAxis);

	// VIEW MATRIX 
	m_xmf4x4View._11 = xmf3Right.x; m_xmf4x4View._12 = xmf3Up.x; m_xmf4x4View._13 = xmf3Look.x;
	m_xmf4x4View._21 = xmf3Right.y; m_xmf4x4View._22 = xmf3Up.y; m_xmf4x4View._23 = xmf3Look.y;
	m_xmf4x4View._31 = xmf3Right.z; m_xmf4x4View._32 = xmf3Up.z; m_xmf4x4View._33 = xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(CurPos, xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(CurPos, xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(CurPos, xmf3Look);

	// PERSPECTIVE / ORTHOGRAPHIC
	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	// INVERSE VIEW 
	m_xmf4x4InverseView._11 = xmf3Right.x; m_xmf4x4InverseView._12 = xmf3Right.y; m_xmf4x4InverseView._13 = xmf3Right.z;
	m_xmf4x4InverseView._21 = xmf3Up.x; m_xmf4x4InverseView._22 = xmf3Up.y; m_xmf4x4InverseView._23 = xmf3Up.z;
	m_xmf4x4InverseView._31 = xmf3Look.x; m_xmf4x4InverseView._32 = xmf3Look.y; m_xmf4x4InverseView._33 = xmf3Look.z;
	m_xmf4x4InverseView._41 = CurPos.x; m_xmf4x4InverseView._42 = CurPos.y; m_xmf4x4InverseView._43 = CurPos.z;

	// FRUSTUM VIEW 
	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void CCamera::SetLookAt(XMFLOAT3& EyePos, XMFLOAT3& LookatPos, XMFLOAT3& xmf3Up)
{
	CGameObject* pOBj = CComponent::GetOwner();

	CTransform* pTransform = (CTransform*)CComponent::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);

	pTransform->SetPosition(EyePos);
	m_xmf4x4View = Matrix4x4::LookAtLH(EyePos, LookatPos, xmf3Up);

	AXIS newAxis = AXIS(XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31),
		XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32),
		XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33));

	XMFLOAT3 fRIght = Vector3::Normalize(newAxis.xmf3Right);
	XMFLOAT3 fUp = Vector3::Normalize(newAxis.xmf3Up);
	XMFLOAT3 fLook = Vector3::Normalize(newAxis.xmf3Look);

	AXIS tAxis = AXIS(fRIght, fUp, fLook);
	pTransform->SetAxis(tAxis);
}





void CCamera::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	/*CGameObject* pOWner    = CComponent::GetOwner();
	CTransform* pTransform = (CTransform*)pOWner->GetComponent(COMPONENT_TYPE::TRANSFORM);
	AXIS tAxis             = pTransform->GetAxis();
	XMFLOAT3 xmf3Pos       = pTransform->GetCurPosition();*/

	// TARGET OBJECT 기준으로 카메라 위치 + 오프셋 위치 세팅 
	CTransform* pTarget_Transform = (CTransform*)m_pTargetObject->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 pTarrgetPos = pTarget_Transform->GetCurPosition();
	AXIS tAxis = pTarget_Transform->GetAxis();

	m_xmf3OffsetFromTargetObj = xmf3CameraOffset;
	XMFLOAT3 CamPos = Vector3::Add(pTarrgetPos, m_xmf3OffsetFromTargetObj);
	SetLookAt(CamPos, pTarrgetPos, tAxis.xmf3Up);
	GenerateViewMatrix();
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	m_fProjectRectDistance = float(1.0f / tan(DegreeToRadian(fFOVAngle * 0.5f)));
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_Viewport.TopLeftX = float(xTopLeft);
	m_Viewport.TopLeftY = float(yTopLeft);
	m_Viewport.Width    = float(nWidth);
	m_Viewport.Height   = float(nHeight);
	m_Viewport.MinDepth = fMinZ;
	m_Viewport.MaxDepth = fMaxZ;
}

void CCamera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	m_ScissorRect.left   = xLeft;
	m_ScissorRect.top    = yTop;
	m_ScissorRect.right  = xRight;
	m_ScissorRect.bottom = yBottom;
}



void CCamera::GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle)
{
	float fAspectRatio = (float(m_Viewport.Width) / float(m_Viewport.Height));
	XMMATRIX xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, xmmtxProjection);

	BoundingFrustum::CreateFromMatrix(m_xmFrustumView, xmmtxProjection);
}

void CCamera::GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float hHeight)
{
	XMMATRIX xmmtxProjection = XMMatrixOrthographicLH(fWidth, hHeight, fNearPlaneDistance, fFarPlaneDistance);
	XMStoreFloat4x4(&m_xmf4x4OrthographicProject, xmmtxProjection);
}

bool CCamera::IsInFrustum(BoundingOrientedBox& xmBoundingBox)
{
	return(m_xmFrustumWorld.Intersects(xmBoundingBox));
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	CTransform* pTransform = (CTransform*)CComponent::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3 vCurPos = pTransform->GetCurPosition();
	XMFLOAT3 vNewPos = Vector3::Add(vCurPos, xmf3Shift);
	pTransform->SetPosition(vNewPos);

}

void CCamera::Move(float x, float y, float z)
{
	XMFLOAT3 vShift = XMFLOAT3(x, y, z);
	Move(vShift);
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	CTransform* pTransform = (CTransform*)CComponent::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
	AXIS tAxis = pTransform->GetAxis();

	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&tAxis.xmf3Right), XMConvertToRadians(fPitch));
		tAxis.xmf3Look = Vector3::TransformNormal(tAxis.xmf3Look, mtxRotate);
		tAxis.xmf3Up = Vector3::TransformNormal(tAxis.xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&tAxis.xmf3Up), XMConvertToRadians(fYaw));
		tAxis.xmf3Look = Vector3::TransformNormal(tAxis.xmf3Look, mtxRotate);
		tAxis.xmf3Right = Vector3::TransformNormal(tAxis.xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&tAxis.xmf3Look), XMConvertToRadians(fRoll));
		tAxis.xmf3Up = Vector3::TransformNormal(tAxis.xmf3Up, mtxRotate);
		tAxis.xmf3Right = Vector3::TransformNormal(tAxis.xmf3Right, mtxRotate);
	}
	pTransform->SetAxis(tAxis);

}

void CCamera::Update(CGameObject* pTargetObj, XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{

	XMFLOAT4X4 mtxRotate = Matrix4x4::Identity();

	// CAMERA 
	CTransform* pCamTransform = (CTransform*)CComponent::GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM);
	XMFLOAT3	xmf3Campos = pCamTransform->GetCurPosition();


	// TARGET 
	CTransform* pTargetTransform = (CTransform*)pTargetObj->GetComponent(COMPONENT_TYPE::TRANSFORM);
	AXIS		tTragetObjAxis = pTargetTransform->GetAxis();
	XMFLOAT3	xmf3TargetCurPos = pTargetTransform->GetCurPosition();

	mtxRotate._11 = tTragetObjAxis.xmf3Right.x; mtxRotate._21 = tTragetObjAxis.xmf3Up.x; mtxRotate._31 = tTragetObjAxis.xmf3Look.x;
	mtxRotate._12 = tTragetObjAxis.xmf3Right.y; mtxRotate._22 = tTragetObjAxis.xmf3Up.y; mtxRotate._32 = tTragetObjAxis.xmf3Look.y;
	mtxRotate._13 = tTragetObjAxis.xmf3Right.z; mtxRotate._23 = tTragetObjAxis.xmf3Up.z; mtxRotate._33 = tTragetObjAxis.xmf3Look.z;

	XMFLOAT3 xmf3Offset = Vector3::TransformCoord(m_xmf3OffsetFromTargetObj, mtxRotate);
	XMFLOAT3 xmf3Position = Vector3::Add(xmf3TargetCurPos, xmf3Offset);
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, xmf3Campos);

	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);

	float fTimeLagScale = fTimeElapsed * (1.0f / 0.25f);
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		xmf3Campos = Vector3::Add(xmf3Campos, xmf3Direction, fDistance);
		SetLookAt(xmf3Campos, xmf3TargetCurPos, tTragetObjAxis.xmf3Up);
	}

	pCamTransform->SetPosition(xmf3Campos);

}

void CCamera::Update(float _fTimeElapsed)
{
	//CTransform* pTransform = (CTransform*)m_pTargetObject->GetComponent(COMPONENT_TYPE::TRANSFORM);
	//XMFLOAT3 TargetPos = pTransform->GetCurPosition();

	//Update(m_pTargetObject, TargetPos);
	GenerateViewMatrix();

}

void CCamera::FinalUpdate(float _fTimeElapsed)
{
}


void CCamera::UpdateViewportsAndScissorRects()
{
	COMMAND_LIST(CGameFramework)->RSSetViewports(1, &m_Viewport);
	COMMAND_LIST(CGameFramework)->RSSetScissorRects(1, &m_ScissorRect);

}

void CCamera::UpdateShaderVariables()
{
	XMFLOAT4X4 xmf4x4View{};
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection{};
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4PerspectiveProject)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	XMFLOAT3 Pos = GetOwner()->Get_Transform_Component()->GetCurPosition();
	::memcpy(&m_pcbMappedCamera->m_xmf3Position, &Pos, sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	COMMAND_LIST(CGameFramework)->SetGraphicsRootConstantBufferView(0, d3dGpuVirtualAddress);



}

void CCamera::CreateShaderVariables()
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbCamera = ResourceManager::GetInst()->CreateBufferResource(NULL
																	, ncbElementBytes
																	, D3D12_HEAP_TYPE_UPLOAD
																	, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
																	, NULL);


	m_pd3dcbCamera->Map(0, NULL, (void**)&m_pcbMappedCamera);
}
