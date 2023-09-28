
#include "stdafx.h"
#include "CGameFramework.h"

#include "CDevice.h"
#include "CCommandQueue.h"
#include "CRootSignature.h"
#include "CSwapChain.h"
#include "CGraphicsPipelineState.h"
#include "CGameObject.h"

#include "ResourceManager.h"

#include "CScene.h"
#include "CFileManager.h"
#include "CTimer.h"
#include "CKeyManager.h"
#include "CImGui.h"



CGameFramework* CGameFramework::m_pInst = nullptr;

CGameFramework::CGameFramework()
{
	
}

CGameFramework::~CGameFramework()
{
	if (m_pCurScene)
		delete m_pCurScene;

		
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{


	m_Window.hInstance        = hInstance;
	m_Window.hWnd             = hMainWnd;
	m_Window.nWndClientHeight = FRAME_BUFFER_HEIGHT;
	m_Window.nWndClientWidth  = FRAME_BUFFER_WIDTH;

///뷰포트를 주 윈도우의 클라이언트 영역 전체로 설정한다. 
	m_d3dScissorRect = { 0, 0, m_Window.nWndClientWidth, m_Window.nWndClientHeight };
	m_d3dViewport    = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };

	/*펜스와 동기화를 위한 이벤트 객체를 생성한다(이벤트 객체의 초기값을 FALSE이다). 이벤트가 실행되면(Signal) 이
	벤트의 값을 자동적으로 FALSE가 되도록 생성한다.*/




	m_Device        = std::make_shared<CDevice>();
	m_CommandQueue  = std::make_shared<CCommandQueue>();
	m_SwapChain     = std::make_shared<CSwapChain>();
	m_RootSignature = std::make_shared<CRootSignature>();
	m_GraphicsPipelineStateMachine = std::make_shared<CGraphicsPipelineState>();


// [ DEVICE ]
	m_Device->OnCreate();

// [ COMMAND QUEUE ]
	m_CommandQueue->OnCreate(m_Window);

// [ SWAPCHAIN ]
	m_SwapChain->OnCreate(m_Window);

// [ ROOT SIGNATURE ]
	m_RootSignature->OnCreate();

#ifdef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	//m_SwapChain->ChangeSwapchainState();
#endif

	ResourceManager::GetInst()->OnCreate();
	BuildObjects();

	CGameTimer::GetInst()->Init();
	CKeyManager::GetInst()->Init();

	CImGui::GetInst()->Init(m_Window);

	return true;


}

void CGameFramework::OnDestroy()
{
	if (m_pCurScene)
		delete m_pCurScene;

	if (m_Device)
		m_Device->~CDevice();

	if (m_CommandQueue)
		m_CommandQueue->~CCommandQueue();

	if (m_SwapChain)
		m_SwapChain->~CSwapChain();

	if (m_RootSignature)
		m_RootSignature->~CRootSignature();

	ResourceManager::GetInst()->OnDestroy();
	FILE_MGR->Destroy();

		
	CImGui::GetInst()->Destroy();



}



void CGameFramework::BuildObjects()
{

	DX12_COMMAND_LIST->Reset(DX12_COMMAND_QUEUE->GetCommandAllocator().Get(), NULL);

	//씬 객체를 생성하고 씬에 포함될 게임 객체들을 생성한다
	m_pCurScene = new CScene;
	m_pCurScene->OnCreate();


	DX12_COMMAND_LIST->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { DX12_COMMAND_LIST.Get() };
	DX12_COMMAND_QUEUE->GetCommandQueue()->ExecuteCommandLists(1, ppd3dCommandLists);

	DX12_COMMAND_QUEUE->WaitForGpuComplete();
	
	m_pCurScene->ReleaseUploadBuffers();


	
}
void CGameFramework::ReleaseObjects()
{
}

void CGameFramework::ProcessInput()
{

}

void CGameFramework::AnimateObjects()
{
}

void CGameFramework::FrameAdvance()
{
/// TIMER  
	CGameTimer::GetInst()->Tick(60.f);
	float fTimeElapsed = CGameTimer::GetInst()->GetTimeElapsed();


/// UPDATE
	if (m_pCurScene)
	{
		CKeyManager::GetInst()->Update();
		m_pCurScene->Update(fTimeElapsed);
		m_pCurScene->FinalUpdate(fTimeElapsed);
	}


/// RENDER 
	m_CommandQueue->Prepare_Rendering();

	if (m_pCurScene) m_pCurScene->Render();
	CImGui::GetInst()->Progress();
	CImGui::GetInst()->Render();

	m_CommandQueue->Execute_Rendering();



	int BackBufferIdx = DX12_SWAP_CHAIN->GetDxgiSwapChain()->GetCurrentBackBufferIndex();

/// TIMER 
	int FPS = CGameTimer::GetInst()->GetFrameRate(m_pszFrameRate + 12, 37);
	std::wstring wstr = L"( " + std::to_wstring(FPS) + L" - FPS ) - 2018180035 장재문";
	::SetWindowText(m_Window.hWnd, wstr.c_str());


/// RENDERING -> FULL SCREEN 터지네..? 
	/*std::shared_ptr<CSwapChain> pSC = DX12_SWAP_CHAIN;
	pSC->ChangeSwapchainState();*/

}



void CGameFramework::WaitForGpuComplete()
{


}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

LRESULT CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}


