#include "stdafx.h"
#include "CScene.h"

#include "CGameObject.h"		/// GAME OBJECT 




CScene::CScene()
{
}

CScene::~CScene()
{

}

void CScene::Init()
{
	
}

void CScene::Animate()
{
}

void CScene::Update()
{
}

void CScene::FinalUpdate()
{
}

void CScene::Render()
{
}



void CScene::AddObject(CGameObject* pObj)
{
	m_vecObjects.push_back(pObj);

}

CGameObject* CScene::GetObjByName(std::string name)
{

	return nullptr;

}

CGameObject* CScene::GetMainCamera()
{
	if (m_pMainCamera)
		return m_pMainCamera;
	else
		return nullptr;

}

void CScene::SetMainCamera(CGameObject* pObj)
{

}


void CScene::CheckObjectByObjectCollisions()
{

}

