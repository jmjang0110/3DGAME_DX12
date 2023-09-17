#pragma once
#include "CEntity.h"

#include "CScript.h"

class CMesh;
class CScript;
class CComponent;
class CMaterial;

class CTransform;
class CCamera;


class CGameObject 
	: public CEntity
{
private:
	std::string								m_PipelineStateName;

private:
	char									m_pstrFrameName[64];

private:
	CGameObject*							m_pParent;
	bool									m_bActive{true};

	std::vector<std::shared_ptr<CMesh>>		m_pMesh;										/// MESH INFO
	std::vector<std::shared_ptr<CMaterial>>	m_pMtrl;										/// MATERIAL INFO 

	std::vector<CGameObject*>				m_vecChild;										/// CHILD INFO
	std::vector<CGameObject*>				m_vecSibling;									/// SIBLING INFO
	std::vector<CScript*>					m_vecScript;									/// SCRIPT INFO
	std::array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> m_arrCom;				/// COMPONENT INFO


	bool						m_bCollide                = false;
	BoundingOrientedBox			m_xmOOBB                  = BoundingOrientedBox();
	CGameObject*				m_pObjectCollided         = NULL;

	float m_fCollideSphere_Radius = 10.f;

public:
	CGameObject();
	~CGameObject();
public:
	void Update(float _fTimeElapsed);
	void FinalUpdate(float _fTimeElapsed);
	void Render();


public:
	void AddScript(CScript* pSCript);
	void AddComponent(CComponent* pCom);
	void AddChild(CGameObject* pObj);


	CGameObject* GetCollideObj() { return m_pObjectCollided; }
	void SetCollideObj(CGameObject* pObj) { m_pObjectCollided = pObj; }
	BoundingOrientedBox GetBoundingBox() { return m_xmOOBB; }

	void SetCollide(bool _B) { m_bCollide = _B; }
	bool IsCollide() { return m_bCollide; }
	void setCollideRadius_Sphere(float _f) { m_fCollideSphere_Radius = _f; }

public:

	/// [ G E T ] 
	CGameObject*				GetParent()							{ return m_pParent; }
	std::vector<CGameObject*>	GetChild()							{ return m_vecChild; }
	CComponent*					GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[static_cast<UINT>(_eType)]; }
	char*						GetFrameName()						{ return m_pstrFrameName; }

	std::shared_ptr<CMesh>		GetMesh() { if (m_pMesh.empty() == false) return m_pMesh[0]; return nullptr; }
	std::vector<std::shared_ptr<CMesh>> GetAllMesh() { return m_pMesh; }

	CTransform* Get_Transform_Component()	{ return (CTransform*)m_arrCom[static_cast<UINT>(COMPONENT_TYPE::TRANSFORM)]; }
	CCamera*	Get_Camera_Component()		{ return (CCamera*)m_arrCom[static_cast<UINT>(COMPONENT_TYPE::CAMERA)]; }
	CGameObject* FindChild(std::string _ChildName);
	std::string GetPipelineStateKeyName() { return m_PipelineStateName; }


	/// [ S E T ]
	void SetMesh(std::shared_ptr<CMesh> pMesh) { m_pMesh.push_back( pMesh); }
	void SetMaterial(int nMaterial, std::shared_ptr<CMaterial> pMaterial);
	void SetPipelineStateKeyName(std::string KeyName) { m_PipelineStateName = KeyName; }

public:
	void ReleaseMeshUploadBuffers();


public:
	void Deactivate();
	void Activate();
	bool IsActive() { return m_bActive; }

	CScript* GetScript(SCRIPT_TYPE _eType);
	bool MyCollideCheck(CGameObject* pObj);
	bool CollideCheck_Sphere(XMFLOAT3 vTargetPos);

};

