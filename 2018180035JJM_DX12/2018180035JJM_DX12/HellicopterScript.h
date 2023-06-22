#pragma once
#include "CScript.h"

//#include "Key.h"

class CGameObject;
class CScene;


class HellicopterScript :
    public CScript
{
private:
    CScene* m_pCurScene;
    CGameObject* m_pAimPoint;

    // MOVE KEY
    KEY         m_eFrontKey         = KEY::W;
    KEY         m_eLeftKey          = KEY::A;
    KEY         m_eBackKey          = KEY::S;
    KEY         m_eRightKey         = KEY::D;

    // ATTACK KEY
    KEY         m_eAttackKey        = KEY::LBTN;
    KEY         m_eCameraMoveKey    = KEY::RBTN;

private:

    UINT                        m_iKillMonsterCount = 0;

    std::vector<CGameObject*>   m_pBullets;
    UINT                        m_iBulletMax         = 50;
    float                       m_fGunRotation_Speed = 1.5;

    // CAMERA
    CGameObject* m_pMainCam;
    XMFLOAT3					m_xmf3OffsetFromTargetObj;
    float                       m_fMaxOffsetDistance = 30.f;
    float                       m_fOffsetHeight      = 10.f;
    float                       m_fRotSpeed          = 1.5f;
    float                       m_fMoveAccRot        = 0.f;

    int                         m_iRotateDir_Past_ForCamAxis = 0;

    XMFLOAT3                    m_xmf3_PastCamRot;


    float m_fMoveSpeed = 50.f;

public:
    std::vector<CGameObject*> GetBullets() { return m_pBullets; }
    void SetAimPointObj(CGameObject* pObj) { m_pAimPoint = pObj; }
    void UpdateAimPoint();


public:
    HellicopterScript();
    HellicopterScript(CGameObject* pOwner);
    virtual ~HellicopterScript() override;

    virtual void Init() override;
    virtual void Update(float _fTimeElapsed) override;

public:
    void UpdateCamera_ThirdPersonMode(float _fTimeElapsed);
    void Rotate_PlayerAxis_Sync_Camera_Axis(float _fTimeElapsed); // 카메라 Front 벡터쪽으로 Player 회전  
    void RotatePlayer(XMFLOAT3 vEndDir, XMFLOAT3 vStartDir, float _fTimeElapsed); // FinalDir 방향으로 Player 가 앞을 바라보게 회전

    float GetRadAngle(XMFLOAT3 _Standard, XMFLOAT3 _Start);
    bool CheckRotateFinish(float _fRot_DT);


public:

    void Update_InputKey(float _fTimeElapsed);
    bool CollideCheck_Bullets(CGameObject* pTarget);




};

