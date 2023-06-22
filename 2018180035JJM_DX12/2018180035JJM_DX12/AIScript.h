#pragma once
#include "CScript.h"

class CScene;
class CHeightmapTerrain;

enum class AI_MOVE_STATE
{
    NONE,
    START,
    ING,
    FINISH,
    END,

};

class AIScript : 
    public CScript
{
private:
    AI_MOVE_STATE m_eMoveState = AI_MOVE_STATE::START;


private:
    CGameObject* m_pTarget;
    CScene* m_pCurScene;
    CGameObject* m_pGun;
    CGameObject* m_pTurret;


    float       m_fRecogDist = 500.f;
    bool        m_bFire;
    float       m_FireTimer = 0.f;
    float       m_FireMaxTime = 0.5f;


    XMFLOAT3    m_vMove2Pos;
    bool        m_bMoveFinish =false;


    // ÃÑ¾Ë 
    std::vector<CGameObject*>   m_pBullets;
    UINT                        m_iBulletMax = 50;
    float                       m_fGunRotation_Speed = 1.5;
public:
    AIScript();
    AIScript(CGameObject* pOwner);
    virtual ~AIScript() override;

    virtual void Init() override;
    virtual void Update(float _fTimeElapsed) override;


public:
    void SetTarget(CGameObject* pObj) { m_pTarget = pObj; }

    void Fire();
    float GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start);
    bool CollideCheck_Bullets(CGameObject* pTarget);

};

