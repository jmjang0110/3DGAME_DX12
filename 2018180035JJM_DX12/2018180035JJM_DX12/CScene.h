#pragma once

#include "Utility.h"

class CGameObject;
class CGraphicsPipelineState;
class CLight;
class CHeightmapTerrain;

class CScene
{
private:
	CLight*						m_pLight;
	CGameObject*				m_pMainCamera;
	std::vector<CGameObject*>	m_vecMonsters;


	std::vector<CGameObject*>	m_vecObjects;
	std::vector<CGameObject*>	m_vecSubCamera;

	CHeightmapTerrain*			m_pTerrain;


private:
	std::multimap<std::string, std::shared_ptr<CGraphicsPipelineState>> m_PipelineStates;

public:
	CScene();
	~CScene();

public:
	void AddTank();
	void AddCamera();
	void AddMonster();
	void AddTerrain();


public:
	void CreateHW2Ojbects();


public:
/// [ FRAME ADVANCE ]
	void OnCreate();
	void Animate(float _fTimeElapsed);
	void Render();

/// [ FRAME ADVANCE SUB FUNCTION ]
	void Update(float _fTimeElapsed);			/// ANIMATE 
	void FinalUpdate(float _fTimeElapsed);		/// ANIMATE 

	void UpdateShaderVariables();
	void ReleaseShaderVariables();


public:
	void PushBack_GameObject(CGameObject* pObj);
	bool Render_PipelineState(std::string PipelineStateName);


/// [ G E T ]
public:
	CGameObject* GetMainCamera();
	CGameObject* GetObjByName(std::string name);
	CHeightmapTerrain* GetTerrain() { return m_pTerrain; }
	std::vector<CGameObject*> GetMonsters() { return m_vecMonsters; }


/// [ S E T ] 
public:
	void SetMainCamera(CGameObject* pObj);

public:
	void CheckObjectByObjectCollisions();

public:
	/// [ ADD GRAPHICS PIPELINE STATE ]
	void AddGraphicsPipelineState(std::string name, CGraphicsPipelineState* pPipeline);
	std::shared_ptr<CGraphicsPipelineState> SetGraphicsPipelineState(std::string name);


	void ReleaseUploadBuffers();
	float RandF(float fMin, float fMax)
	{
		return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
	}
};

