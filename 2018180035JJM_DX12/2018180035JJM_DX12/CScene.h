#pragma once

#include "Utility.h"

class CGameObject;

class CScene
{
private:
	std::vector<CGameObject*>	m_vecObjects;
	CGameObject*				m_pMainCamera;


public:
	CScene();
	~CScene();

public:
/// [ FRAME ADVANCE ]
	void Init();
	void Animate();
	void Render();

/// [ FRAME ADVANCE SUB FUNCTION ]
	void Update();			/// ANIMATE 
	void FinalUpdate();		/// ANIMATE 

public:
	void AddObject(CGameObject* pObj);

/// [ G E T ]
public:
	CGameObject* GetMainCamera();
	CGameObject* GetObjByName(std::string name);


/// [ S E T ] 
public:
	void SetMainCamera(CGameObject* pObj);

public:
	void CheckObjectByObjectCollisions();



};

