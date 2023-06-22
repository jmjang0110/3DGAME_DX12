
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

///����Ʈ�� �� �������� Ŭ���̾�Ʈ ���� ��ü�� �����Ѵ�. 
	m_d3dScissorRect = { 0, 0, m_Window.nWndClientWidth, m_Window.nWndClientHeight };
	m_d3dViewport    = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };

	/*�潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� �����Ѵ�(�̺�Ʈ ��ü�� �ʱⰪ�� FALSE�̴�). �̺�Ʈ�� ����Ǹ�(Signal) ��
	��Ʈ�� ���� �ڵ������� FALSE�� �ǵ��� �����Ѵ�.*/




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
	CFileManager::GetInst()->Destroy();

	


}



void CGameFramework::BuildObjects()
{

	COMMAND_LIST(CGameFramework)->Reset(COMMAND_QUEUE(CGameFramework)->GetCommandAllocator().Get(), NULL);

	//�� ��ü�� �����ϰ� ���� ���Ե� ���� ��ü���� �����Ѵ�
	m_pCurScene = new CScene;
	m_pCurScene->OnCreate();


	COMMAND_LIST(CGameFramework)->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { COMMAND_LIST(CGameFramework).Get() };
	COMMAND_QUEUE(CGameFramework)->GetCommandQueue()->ExecuteCommandLists(1, ppd3dCommandLists);

	COMMAND_QUEUE(CGameFramework)->WaitForGpuComplete();
	
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
	if(m_pCurScene) m_pCurScene->Render();
	m_CommandQueue->Execute_Rendering();

	

/// TIMER 
	int FPS = CGameTimer::GetInst()->GetFrameRate(m_pszFrameRate + 12, 37);
	std::wstring wstr = L"( " + std::to_wstring(FPS) + L" - FPS ) - 2018180035 ���繮";
	::SetWindowText(m_Window.hWnd, wstr.c_str());


/// RENDERING -> FULL SCREEN ������..? 
	/*std::shared_ptr<CSwapChain> pSC = SWAP_CHAIN(CGameFramework);
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


