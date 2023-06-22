#pragma once

class CGameObject;

class RotateScript : 
    public CScript
{
private:
    CGameObject* m_pTarget{};
    float m_fSpeed = 100.f;




public:
    RotateScript();
    RotateScript(CGameObject* pOwner);
    virtual ~RotateScript() override;

    virtual void Init() override;
    virtual void Update(float _fTimeElapsed) override;

public:
    void SetTargetObject(CGameObject* pObj) { m_pTarget = pObj; }



};

