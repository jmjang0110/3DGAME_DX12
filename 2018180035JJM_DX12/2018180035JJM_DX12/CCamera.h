#pragma once

#include "CGameObject.h"
#include "CComponent.h"

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

#define FIRST_PERSON_CAMERA			0x01
#define SPACESHIP_CAMERA			0x02
#define THIRD_PERSON_CAMERA			0x03

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
	XMFLOAT3						m_xmf3Position;
};

class CCamera : public CComponent
{
private:
	ComPtr<ID3D12Resource>		m_pd3dcbCamera{};
	VS_CB_CAMERA_INFO*			m_pcbMappedCamera{};
private:
	DWORD						m_nMode;

	float						m_fFOVAngle = 90.0f;
	float						m_fProjectRectDistance = 1.0f;
	float						m_fAspectRatio = float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT);

	// MATRIX 
	XMFLOAT4X4					m_xmf4x4View        = Matrix4x4::Identity();	/// VIEW 
	XMFLOAT4X4					m_xmf4x4InverseView = Matrix4x4::Identity();	/// INVERSE VIEW 

	XMFLOAT4X4					m_xmf4x4PerspectiveProject     = Matrix4x4::Identity();	/// PERSPECTIVE 
	XMFLOAT4X4					m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();	/// VIEW * PERSPECTIVE 

	XMFLOAT4X4					m_xmf4x4OrthographicProject     = Matrix4x4::Identity();	/// ORTHOGRAPHIC
	XMFLOAT4X4					m_xmf4x4ViewOrthographicProject = Matrix4x4::Identity();	/// VIEW * ORTHOGRAPHIC

	// VIEWPORT
	D3D12_VIEWPORT				m_Viewport;
	D3D12_RECT					m_ScissorRect;

private:
	// FRUSTUM 
	BoundingFrustum				m_xmFrustumView = BoundingFrustum();
	BoundingFrustum				m_xmFrustumWorld = BoundingFrustum();

	// OFFSET POS 
	CGameObject* m_pTargetObject;
	XMFLOAT3					m_xmf3OffsetFromTargetObj;

public:
	/// [ S E T ]
	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);
	void SetTargetObject(CGameObject* pObj) { m_pTargetObject = pObj; }

	void SetFOVAngle(float fFOVAngle);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	

	void SetLookAt(XMFLOAT3& EyePos, XMFLOAT3& LookatPos, XMFLOAT3& xmf3Up);
	void SetMode(DWORD nMode) { m_nMode = nMode; }


	/// [ G E T ]
	XMFLOAT4X4 GetPerspectiveProjectMat() { return m_xmf4x4PerspectiveProject; }
	XMFLOAT4X4 GetViewPerspectiveProjectMat() { return m_xmf4x4ViewPerspectiveProject; }
	D3D12_VIEWPORT GetViewport() { return m_Viewport; }
	DWORD GetMode() { return(m_nMode); }

public:
	/// [ G E N E R A T E ]
	void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float hHeight);


	/// [ U T I L I T Y ]
	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(CGameObject* pTargetObj, XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);
	void Update(float _fTimeElapsed = 0.016f) override;
	void FinalUpdate(float _fTimeElapsed) override;
	bool IsInFrustum(BoundingOrientedBox& xmBoundingBox);

public:
	void UpdateViewportsAndScissorRects();
	void UpdateShaderVariables() override;

	void CreateShaderVariables();

public:
	CCamera();
	virtual ~CCamera();
};

