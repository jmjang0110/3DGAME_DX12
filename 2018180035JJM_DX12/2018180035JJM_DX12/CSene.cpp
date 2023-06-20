#include "stdafx.h"
#include "CScene.h"

#include "CGameObject.h"		/// GAME OBJECT 
#include "CLight.h"				/// LIGHT
#include "CGraphicsPipelineState.h"

#include "CCommandQueue.h"
#include "CRootSignature.h"

#include "ResourceManager.h"
#include "CFileManager.h"
#include "CGameFramework.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CCamera.h"

#include "CScript.h"
#include "CameraScript.h"
#include "PlayerScript.h"
#include "TankMonster.h"

#include "Utility.h"





CScene::CScene()
{
}

CScene::~CScene()
{
	STL_VECTOR_DELETE(m_vecObjects)
	
	if (m_pLight)
		delete m_pLight;

	if (m_pMainCamera)
		delete m_pMainCamera;

	for (auto iter = m_PipelineStates.begin(); iter != m_PipelineStates.end(); ++iter) {
		iter->second.reset();

	}

}

void CScene::OnCreate()
{
	m_pLight = new CLight;
	m_pLight->OnCreate();
	AddCamera();


	CGameObject* pModel = CFileManager::GetInst()->LoadGeometryFromFile("Model/Apache.bin");
	//pObj->SetName("Apache");
	//CTransform* TransCom = new CTransform;
	//TransCom->SetPosition(0.f, 1.f, -50.f);
	//TransCom->SetScale(0.1f, 0.1f, 0.1f);
	//pObj->AddComponent(TransCom);
	//PushBack_GameObject(pObj);


	CGameObject* pBullet = new CGameObject;
	pBullet->SetName("skin");
	pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("skin"));
	pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));
	CTransform* TransCom = new CTransform;
	TransCom->SetPosition(RandF(-10.f, 10.f), RandF(-50.f, 50.f), RandF(-50.f, 100.f));
	pBullet->AddComponent(TransCom);
	PushBack_GameObject(pBullet);
	
	pBullet = new CGameObject;
	pBullet->SetName("skin");
	pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("skin"));
	pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));
	TransCom = new CTransform;
	TransCom->SetPosition(RandF(-10.f, 10.f), RandF(-50.f, 50.f), RandF(-50.f, 100.f));
	pBullet->AddComponent(TransCom);
	PushBack_GameObject(pBullet);

	pBullet = new CGameObject;
	pBullet->SetName("skin");
	pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("skin"));
	pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));
	TransCom = new CTransform;
	TransCom->SetPosition(RandF(-10.f, 10.f), RandF(-50.f, 50.f), RandF(-50.f, 100.f));
	pBullet->AddComponent(TransCom);
	PushBack_GameObject(pBullet);

	pBullet = new CGameObject;
	pBullet->SetName("skin");
	pBullet->SetMesh(ResourceManager::GetInst()->GetMesh("skin"));
	pBullet->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("skin0"));
	TransCom = new CTransform;
	TransCom->SetPosition(RandF(-10.f, 10.f), RandF(-50.f, 50.f), RandF(-50.f, 100.f));
	pBullet->AddComponent(TransCom);
	PushBack_GameObject(pBullet);
	
	
	
	AddTank();
	//AddMonster();


	SAFE_DELETE(pModel)

		CGameObject*  pObj = new CGameObject;
	pObj->SetName("Cube");
	pObj->SetMesh(ResourceManager::GetInst()->GetMesh("Cube"));
	pObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("black_m_70"));

	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 0.f, -50.f);
	TransCom->SetScale(0.5f, 0.5f, 0.5f);

	pObj->AddComponent(TransCom);
	PushBack_GameObject(pObj);


	//std::vector<CGameObject*> pSceneLoad;
	//pSceneLoad = CFileManager::GetInst()->LoadSceneObjectsFromFile((char*)"Model/Scene.bin");
	//
	//STL_VECTOR_DELETE(pSceneLoad);
	//return;

	//int i = 0;
	//for (auto iter : pSceneLoad ) {
	//	iter->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("silver0"));
	//	PushBack_GameObject(iter);
	//	i++;
	//	if (i >= 600)
	//		break;
	//}
}


void CScene::AddTank()
{
	CGameObject* pObj = CFileManager::GetInst()->LoadGeometryFromFile("Model/Gunship.bin");
	pObj->SetName("Gunship");
	CTransform* TransCom = new CTransform;
	TransCom->SetPosition(0.f, 1.f, -50.f);
	pObj->AddComponent(TransCom);
	SAFE_DELETE(pObj);


	pObj = new CGameObject;
	pObj->SetName("TankObject");
	MeshLoadInfo* pTankModelInfo = CFileManager::GetInst()->LoadMeshInfo_Tank("Model/tank_body.bin");
	if (pTankModelInfo)
	{
		std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
		if (pTankModelInfo->m_nType & VERTEXT_NORMAL)
		{
			pMesh = ResourceManager::GetInst()->GetMesh("tank_body");

			if (pMesh == nullptr) {
				pMesh = std::make_shared<CMesh>();
				pMesh->OnCreate(pTankModelInfo);
				pMesh->CreateNormalBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_body", pMesh);
			}
		}
		if (pMesh) {
			pMesh->CreateIndexBufferResource(pTankModelInfo);
			pObj->SetMesh(pMesh);
		}
	}
	pObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 0.f, -0.f);
	pObj->AddComponent(TransCom);


	PlayerScript* pPlayerSC = new PlayerScript;
	pObj->AddScript(pPlayerSC);
	PushBack_GameObject(pObj);

	SAFE_DELETE(pTankModelInfo);



	CGameObject* pObj_turret = new CGameObject;
	pObj_turret->SetName("Turret");
	pTankModelInfo = CFileManager::GetInst()->LoadMeshInfo_Tank("Model/tank_turret.bin");
	if (pTankModelInfo)
	{
		std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
		if (pTankModelInfo->m_nType & VERTEXT_NORMAL)
		{
			pMesh = ResourceManager::GetInst()->GetMesh("tank_turret");

			if (pMesh == nullptr) {
				pMesh = std::make_shared<CMesh>();
				pMesh->OnCreate(pTankModelInfo);
				pMesh->CreateNormalBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_turret", pMesh);
			}
		}
		if (pMesh) {
			pMesh->CreateIndexBufferResource(pTankModelInfo);
			pObj_turret->SetMesh(pMesh);
		}
	}
	pObj_turret->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 3.5f, -0.f);
	TransCom->IgnoreParentRotation(true);
	pObj_turret->AddComponent(TransCom);
	pObj->AddChild(pObj_turret);
	//PushBack_GameObject(pObj_turret);

	SAFE_DELETE(pTankModelInfo);



	CGameObject* pObj_gun = new CGameObject;
	pObj_gun->SetName("Gun");
	pTankModelInfo = CFileManager::GetInst()->LoadMeshInfo_Tank("Model/tank_gun.bin");
	if (pTankModelInfo)
	{
		std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
		if (pTankModelInfo->m_nType & VERTEXT_NORMAL)
		{
			pMesh = ResourceManager::GetInst()->GetMesh("tank_gun");

			if (pMesh == nullptr) {
				pMesh = std::make_shared<CMesh>();
				pMesh->OnCreate(pTankModelInfo);
				pMesh->CreateNormalBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_gun", pMesh);
			}
		}
		if (pMesh) {
			pMesh->CreateIndexBufferResource(pTankModelInfo);
			pObj_gun->SetMesh(pMesh);
		}
	}
	pObj_gun->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 1.5f, -0.f);
	TransCom->IgnoreParentRotation(true);
	pObj_gun->AddComponent(TransCom);
	pObj_turret->AddChild(pObj_gun);
	//PushBack_GameObject(pObj_gun);
	SAFE_DELETE(pTankModelInfo);



	pPlayerSC->Init();

	return;

}

void CScene::AddCamera()
{
	m_pMainCamera = new CGameObject{};
	m_pMainCamera->SetName("MainCamera");
	CTransform* TransCom = new CTransform;
	TransCom->SetPosition(0.f, 20, 0.f);

	CCamera* pCamCom = new CCamera;
	pCamCom->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	pCamCom->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	pCamCom->GeneratePerspectiveProjectionMatrix(1.01f, 5000.f, 60.f);
	pCamCom->SetFOVAngle(60.f);
	pCamCom->GenerateOrthographicProjectionMatrix(1.f, 50.f, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	pCamCom->CreateShaderVariables();

	//CameraScript* pCamScript = new CameraScript;
	//m_pMainCamera->AddScript(pCamScript);

	m_pMainCamera->AddComponent(TransCom);
	m_pMainCamera->AddComponent(pCamCom);

}

void CScene::AddMonster()
{

	CGameObject* pObj = new CGameObject;
	pObj->SetName("TankMonster");
	pObj->SetMesh(ResourceManager::GetInst()->GetMesh("tank_body"));
	pObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	CTransform* TransCom = new CTransform;
	TransCom->SetPosition(0.f, 0.f, 50.f);
	pObj->AddComponent(TransCom);
	PushBack_GameObject(pObj);


	CGameObject* pObj_turret = new CGameObject;
	pObj_turret->SetName("TurretMonster");
	pObj_turret->SetMesh(ResourceManager::GetInst()->GetMesh("tank_turret"));
	pObj_turret->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 3.5f, -0.f);
	TransCom->IgnoreParentRotation(true);
	pObj_turret->AddComponent(TransCom);
	pObj->AddChild(pObj_turret);

	CGameObject* pObj_gun = new CGameObject;
	pObj_gun->SetName("GunMontser");
	pObj_gun->SetMesh(ResourceManager::GetInst()->GetMesh("tank_gun"));
	pObj_gun->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(0.f, 1.5f, -0.f);
	TransCom->IgnoreParentRotation(true);
	pObj_gun->AddComponent(TransCom);
	pObj_turret->AddChild(pObj_gun);
	

	TankMonsterScript* pMonsterSc = new TankMonsterScript;
	pObj->AddScript(pMonsterSc);
	TankMonsterScript* pMonScript_Gun = new TankMonsterScript;
	pObj_gun->AddScript(pMonScript_Gun);

	pMonsterSc->Init();
	pMonScript_Gun->Init();
}



void CScene::Animate(float _fTimeElapsed)
{
}

void CScene::Update(float _fTimeElapsed)
{
	for (auto pObj : m_vecObjects) {
		if(pObj->IsActive())
			pObj->Update(_fTimeElapsed);
	}

	if (m_pMainCamera)
		m_pMainCamera->Update(_fTimeElapsed);
}

void CScene::FinalUpdate(float _fTimeElapsed)
{
	for (auto pObj : m_vecObjects) {
		if (pObj->IsActive())
			pObj->FinalUpdate(_fTimeElapsed);
	}

	if (m_pMainCamera)
		m_pMainCamera->FinalUpdate(_fTimeElapsed);


}

void CScene::UpdateShaderVariables()
{
	if(m_pLight) 
		m_pLight->UpdateShaderVariables();
}

void CScene::ReleaseShaderVariables()
{
	if (m_pLight) 
		m_pLight->ReleaseShaderVariables();

}

void CScene::Render()
{
	std::shared_ptr<CGraphicsPipelineState> pCurPipeline = std::make_shared<CGraphicsPipelineState>();
	pCurPipeline = ResourceManager::GetInst()->GetPipelineState("Illuminate"); /// Illuminate 이름의 PipelineState 를 세팅한다. 
	
	if (pCurPipeline.get())
	{
		/// 현재 파이프라인으로 렌더링할 오브젝트 들을 집어넣는다 --> 현재 TEST로 그냥 다 집어넣는다. 
		for (auto pObj : m_vecObjects) {
			if(pObj->IsActive())
				pCurPipeline->PushBack_GameObject(pObj);
		}
	}
	
	COMMAND_LIST(CGameFramework)->SetGraphicsRootSignature(ROOT_SIGNATURE(CGameFramework).Get());
	if (m_pMainCamera)
	{
		m_pMainCamera->Get_Camera_Component()->UpdateViewportsAndScissorRects();
		m_pMainCamera->Get_Camera_Component()->UpdateShaderVariables();
	}

	UpdateShaderVariables();

	if (m_pLight)
		m_pLight->UpdateGraphicsRootConstantBufferView();

	pCurPipeline->SetPipelineState();
	pCurPipeline->ExecuteRender();		/// 저장한 오브젝트들을 렌더링 한다. 
	pCurPipeline->Clear_GameObject();
}



void CScene::PushBack_GameObject(CGameObject* pObj)
{
	m_vecObjects.push_back(pObj);

}

CGameObject* CScene::GetObjByName(std::string name)
{
	CGameObject* pChild = nullptr;


	for (auto pObj : m_vecObjects) {

		if (name == pObj->GetName()) {
			pChild = pObj;
			break;
		}

		pChild = pObj->FindChild(name);
		if (pChild)
			break;

	}

	return pChild;

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

void CScene::AddGraphicsPipelineState(std::string name, CGraphicsPipelineState* pPipeline)
{
	m_PipelineStates.insert(std::make_pair(name, pPipeline));
}

std::shared_ptr<CGraphicsPipelineState> CScene::SetGraphicsPipelineState(std::string name)
{
	std::multimap<std::string, std::shared_ptr<CGraphicsPipelineState>>::iterator Iter = m_PipelineStates.find(name);
	if (Iter != m_PipelineStates.end()) {
		std::shared_ptr<CGraphicsPipelineState> pPipeline = Iter->second;
		pPipeline->SetPipelineState();
		return pPipeline;

	}

	return nullptr;

}

void CScene::ReleaseUploadBuffers()
{
	for (auto pObj : m_vecObjects) {
		pObj->ReleaseMeshUploadBuffers();
	}
}

