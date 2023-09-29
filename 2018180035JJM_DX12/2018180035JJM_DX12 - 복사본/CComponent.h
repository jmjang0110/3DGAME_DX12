#pragma once

class CGameObject;
class CComponent
{
private:

	bool			m_bActive{true};

	COMPONENT_TYPE	m_eType;
	CGameObject*	m_pOwner;

public:
	CComponent();
	virtual ~CComponent();

public:

	virtual void Init();
	virtual void Update(float _fTimeElapsed);
	virtual void FinalUpdate(float _fTimeElapsed);
	virtual void UpdateShaderVariables();


	void Activate();
	void Deactivate();
	bool IsActive();

public:
	/// [ G E T ]
	COMPONENT_TYPE GetComponentType()	{ return m_eType; }
	CGameObject* GetOwner()				{ return m_pOwner; }


	/// [ S E T ]
	void SetComponentType(COMPONENT_TYPE eType) { m_eType = eType; }
	void SetOwner(CGameObject* pOwner)			{ m_pOwner = pOwner; }

	friend class CGameObject;

};

