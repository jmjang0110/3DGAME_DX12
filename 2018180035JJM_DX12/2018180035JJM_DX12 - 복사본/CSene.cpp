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


#include "CHeightmapGrid.h"
#include "CHeightmapTerrain.h"
#include "CHeightMapImage.h"

#include "RotateScript.h"
#include "HellicopterScript.h"

#include "AIScript.h"

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
	//ResourceManager::GetInst()->CreateSphereMesh();


	//CreateHW2Ojbects();
	//return;

	m_pLight = new CLight;
	m_pLight->OnCreate();
	AddCamera();

	CGameObject * PGunShip = FILE_MGR->LoadGeometryFromFile("Model/Mi24.bin");
	MeshLoadInfo* pTankModelInfo = FILE_MGR->LoadMeshInfo_Tank("Model/tank_body.bin");
	CGameObject* pModel          = FILE_MGR->LoadGeometryFromFile("Model/Apache.bin");
	//CGameObject* pPepper		= FILE_MGR->LoadGeometryFromFile("Model/Bell_pepper_green.bin");
	
	ResourceManager::GetInst()->CreateCubeMesh(10, 10, 10);


	CGameObject* CubeObj = new CGameObject;
	CubeObj->SetPipelineStateKeyName("Basic");
	CubeObj->SetName("Cube");
	CubeObj->SetMesh(ResourceManager::GetInst()->GetMesh("Cube"));

	ResourceManager::GetInst()->GetMaterial("rotor0")->SetTexture(RES_MGR->GetTexture("Image/Stone.dds"));
	CubeObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	
	CTransform* CubeTrans = new CTransform;
	CubeTrans->SetScale(10.f, 10.f, 10.f);
	CubeTrans->SetPosition(50.f, 0.f, 0.f);
	CubeObj->AddComponent(CubeTrans);
	PushBack_GameObject(CubeObj);

	//AddTerrain();


	SAFE_DELETE(PGunShip);
	//SAFE_DELETE(pModel);
	SAFE_DELETE(pTankModelInfo);

}


void CScene::AddTank()
{
	CGameObject* pObj = FILE_MGR->LoadGeometryFromFile("Model/Gunship.bin");
	pObj->SetName("Gunship");
	CTransform* TransCom = new CTransform;
	TransCom->SetPosition(0.f, 0.f, -0.f);
	pObj->AddComponent(TransCom);
	SAFE_DELETE(pObj);


	pObj = new CGameObject;
	pObj->SetName("TankObject");
	pObj->SetPipelineStateKeyName("Illuminate");
	
	m_vecMonsters.push_back(pObj);


	int Width = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapWidth();
	int Length = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapLength();
	XMFLOAT3 scale = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetScale();

	float PosX = rand() % (int)(Width * scale.x);
	float PosZ = rand() % (int)(Length * scale.z);



	MeshLoadInfo* pTankModelInfo = FILE_MGR->LoadMeshInfo_Tank("Model/tank_body.bin");
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
				pMesh->CreateIndexBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_body", pMesh);
			}
		}
		if (pMesh) {
			
			pObj->SetMesh(pMesh);
		}
	}
	pObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	TransCom = new CTransform;
	TransCom->SetPosition(PosX, 0.f, PosZ);
	pObj->AddComponent(TransCom);


	//PlayerScript* pPlayerSC = new PlayerScript;
	//pObj->AddScript(pPlayerSC);
	AIScript* pAI = new AIScript;
	pAI->SetTarget(GetObjByName("Player"));
	pObj->AddScript(pAI);

	PushBack_GameObject(pObj);

	SAFE_DELETE(pTankModelInfo);



	CGameObject* pObj_turret = new CGameObject;
	pObj_turret->SetName("Turret");
	pTankModelInfo = FILE_MGR->LoadMeshInfo_Tank("Model/tank_turret.bin");
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
				pMesh->CreateIndexBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_turret", pMesh);
			}
		}
		if (pMesh) {
			
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
	pTankModelInfo = FILE_MGR->LoadMeshInfo_Tank("Model/tank_gun.bin");
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
				pMesh->CreateIndexBufferResource(pTankModelInfo);
				ResourceManager::GetInst()->AddMesh("tank_gun", pMesh);
			}
		}
		if (pMesh) {

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


	pAI->Init();

	//pPlayerSC->Init();

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

	CameraScript* pCamScript = new CameraScript;
	m_pMainCamera->AddScript(pCamScript);

	m_pMainCamera->AddComponent(TransCom);
	m_pMainCamera->AddComponent(pCamCom);

}

void CScene::AddMonster()
{

	CGameObject* pObj = new CGameObject;
	pObj->SetPipelineStateKeyName("Illuminate");

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

void CScene::AddTerrain()
{
	if (m_pTerrain)
		return;

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 8배, y-축은 2배 확대한다. 
	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.1f, 0.4f, 0.0f, 0.0f);

	//지형을 높이 맵 이미지 파일(HeightMap.raw)을 사용하여 생성한다. 높이 맵의 크기는 가로x세로(257x257)이다. 
#ifdef _WITH_TERRAIN_PARTITION
	m_pTerrain = new CHeightmapTerrain(
		_T("Image/HeightMap.raw")
		, 257
		, 257
		, 17
		, 17
		, xmf3Scale
		, xmf4Color);

#else
//지형을 하나의 격자 메쉬(257x257)로 생성한다. 
	m_pTerrain = new CHeightmapTerrain(
	 _T("Image/HeightMap.raw")
		, 257
		, 257
		, 257
		, 257
		, xmf3Scale
		, xmf4Color);

#endif
	CGameObject* pObj = m_pTerrain->GetTerrainObject();
	pObj->SetPipelineStateKeyName("Basic");

	PushBack_GameObject(pObj);

}

void CScene::CreateHW2Ojbects()
{
	m_pLight = new CLight;
	m_pLight->OnCreate();
	AddCamera();

	CGameObject* PGunShip = FILE_MGR->LoadGeometryFromFile("Model/Mi24.bin");
	MeshLoadInfo* pTankModelInfo = FILE_MGR->LoadMeshInfo_Tank("Model/tank_body.bin");
	CGameObject* pModel = FILE_MGR->LoadGeometryFromFile("Model/Apache.bin");


	ResourceManager::GetInst()->CreateCubeMesh(10, 10, 10);
	ResourceManager::GetInst()->CreateSphereMesh();

	AddTerrain();

	for (int i = 0; i < 20; ++i)
		AddTank();

	CGameObject* CubeObj = new CGameObject;
	CubeObj->SetPipelineStateKeyName("Basic");
	CubeObj->SetName("Sphere");
	CubeObj->SetMesh(ResourceManager::GetInst()->GetMesh("Cube"));
	CubeObj->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	CTransform* CubeTrans = new CTransform;
	CubeTrans->SetScale(0.2f, 0.2f, 0.2f);
	CubeTrans->SetPosition(50.f, 0.f, 0.f);
	CubeObj->AddComponent(CubeTrans);
	PushBack_GameObject(CubeObj);



	CGameObject* CubeObj2 = new CGameObject;
	CubeObj2->SetPipelineStateKeyName("Basic");
	CubeObj2->SetName("Cube2");
	CubeObj2->SetMesh(ResourceManager::GetInst()->GetMesh("tank_body"));
	CubeObj2->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));

	CTransform* Cube2Trans = new CTransform;
	Cube2Trans->SetPosition(-50.f, 0.f, 0.f);
	CubeObj2->AddComponent(Cube2Trans);
	PushBack_GameObject(CubeObj2);


	//std::shared_ptr<CMesh> cube = ResourceManager::GetInst()->GetMesh("Cube");
	std::shared_ptr<CMesh> Other = ResourceManager::GetInst()->GetMesh("tank_body");


	int Width = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapWidth();
	int Length = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeightMapLength();
	XMFLOAT3 scale = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetScale();

	float PosX = rand() % (int)(Width * scale.x);
	float PosZ = rand() % (int)(Length * scale.z);
	float PosY = CGameFramework::GetInst()->GetCurScene()->GetTerrain()->GetHeight(PosX, PosZ);

	CGameObject* pObj_Body = new CGameObject;
	pObj_Body->SetName("Player");
	pObj_Body->SetPipelineStateKeyName("Illuminate");
	pObj_Body->SetMesh(ResourceManager::GetInst()->GetMesh("Gunship_Instance"));
	pObj_Body->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	CTransform* pModelTrans = new CTransform;
	pModelTrans->SetPosition(PosX, PosY + 50.f, PosZ);
	pObj_Body->AddComponent(pModelTrans);

	HellicopterScript* pHelSCript = new HellicopterScript(pObj_Body);
	pHelSCript->SetAimPointObj(CubeObj);
	pHelSCript->Init();
	pObj_Body->AddScript(pHelSCript);
	PushBack_GameObject(pObj_Body);



	CGameObject* pObj_Rotor = new CGameObject;
	pObj_Rotor->SetPipelineStateKeyName("Illuminate");
	pObj_Rotor->SetMesh(ResourceManager::GetInst()->GetMesh("Rotor_Instance"));
	pObj_Rotor->SetMaterial(0, ResourceManager::GetInst()->GetMaterial("rotor0"));
	CTransform* pRotorTRans = new CTransform;
	pRotorTRans->SetPosition(0.f, 5.f, 0.f);

	RotateScript* pRotScript = new RotateScript;
	pRotScript->Init();
	pRotScript->SetTargetObject(pObj_Rotor);
	pObj_Rotor->AddScript(pRotScript);
	pObj_Rotor->AddComponent(pRotorTRans);

	pObj_Body->AddChild(pObj_Rotor);






	//for (int i = 0; i < 10; ++i)
	//{
	//	AddTank();
	//}
	SAFE_DELETE(PGunShip);
	SAFE_DELETE(pModel);
	SAFE_DELETE(pTankModelInfo);
	return;
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

	
	DX12_COMMAND_LIST->SetGraphicsRootSignature(DX12_ROOT_SIGNATURE.Get());
	if (m_pMainCamera)
	{
		m_pMainCamera->Get_Camera_Component()->UpdateViewportsAndScissorRects();
		m_pMainCamera->Get_Camera_Component()->UpdateShaderVariables();
	}

	UpdateShaderVariables();

	if (m_pLight)
		m_pLight->UpdateGraphicsRootConstantBufferView();

	Render_PipelineState("Illuminate");
	Render_PipelineState("Basic");

}



void CScene::PushBack_GameObject(CGameObject* pObj)
{
	m_vecObjects.push_back(pObj);

}

bool CScene::Render_PipelineState(std::string PipelineStateName)
{
	std::shared_ptr<CGraphicsPipelineState> pCurPipeline = std::make_shared<CGraphicsPipelineState>();
	pCurPipeline = ResourceManager::GetInst()->GetPipelineState(PipelineStateName); /// Illuminate 이름의 PipelineState 를 세팅한다. 
	if (pCurPipeline == nullptr)
		return false;


	/// 현재 파이프라인으로 렌더링할 오브젝트 들을 집어넣는다 ( Object 의 PipelineStateName으로..)
	for (auto pObj : m_vecObjects) {
		if (pObj->IsActive())
		{
			if(pObj->GetPipelineStateKeyName() == PipelineStateName)
				pCurPipeline->PushBack_GameObject(pObj);
		}
	}
	

	pCurPipeline->SetPipelineState();
	pCurPipeline->ExecuteRender();		/// 저장한 오브젝트들을 렌더링 한다. 
	pCurPipeline->Clear_GameObject();

	return true;

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

