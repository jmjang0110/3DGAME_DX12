#pragma once

#include "CScript.h"

class CameraScript :
	public CScript
{
private:
	float                       m_fMaxOffsetDistance = 20.f;
	float                       m_fOffsetHeight      = 10.f;
	float                       m_fRotSpeed          = 2.f;

public:
	CameraScript();
	CameraScript(CGameObject* pOwner);
	virtual ~CameraScript() override;


	virtual void Init();
	virtual void Update(float _fTimeElapsed) override;




};

