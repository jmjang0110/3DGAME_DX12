#pragma once
#include "CScript.h"
class CScene;
class CGameObject;

class TankMonsterScript :
    public CScript
{
private:
    float           m_fTimer = 0.f;

    bool m_bFinish = false;

    CGameObject* m_pTargetObj;


    float       m_fMoveSpeed = 10.f;
    XMFLOAT3    m_fMoveFrontDir;
    int         m_iRotateDir_ToPastAxis = 0;

    CScene* m_pCurScene;
    CGameObject* m_pGun;
    CGameObject* m_pTurret;
    CGameObject* m_pBody;


    int  m_iBulletMax = 20;
    std::vector<CGameObject*>   m_pBullets;
    XMFLOAT3                    m_xmf3_PastTargetRot;
    float m_fAcc = 0.f;
    int  m_iMoveDir = 1;

    int m_iHitCnt = 0;

public:
    TankMonsterScript();
    TankMonsterScript(CGameObject* pOwner);
    virtual ~TankMonsterScript() override;

    virtual void Init() override;
    virtual void Update(float _fTimeElapsed) override;


    std::vector<CGameObject*> GetBullets() { return m_pBullets; }


    void Start();
    void RotateToPlayer(XMFLOAT3 vEndDir, XMFLOAT3 vStartDir, float _fTimeElapsed);
    void UpdateMove(float _fTimeElapsed);
    float GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start);

    void setTarget(CGameObject* pObj) { m_pTargetObj = pObj; }
    void UpdatePosition(float _fTimeElapsed);

  
    void Fire();
    bool CollideCheck_Bullets(CGameObject* pTarget);


};

