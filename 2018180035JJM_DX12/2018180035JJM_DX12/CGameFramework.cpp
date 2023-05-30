
#include "stdafx.h"
#include "CGameFramework.h"

#include "CDevice.h"
#include "CCommandQueue.h"
#include "CRootSignature.h"
#include "CSwapChain.h"
#include "CGraphicsPipelineState.h"
#include "CGameObject.h"


CGameFramework* CGameFramework::m_pInst = nullptr;

CGameFramework::CGameFramework()
{
	
}

CGameFramework::~CGameFramework()
{
	if (m_pTestObj)
		delete m_pTestObj;

}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	int aasdf = 123;


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

// [ DX12 PIPELINE STATE ]
	m_GraphicsPipelineStateMachine->OnCreate();

	m_pTestObj = new CGameObject;


#ifdef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	//m_SwapChain->ChangeSwapchainState();
#endif


	return true;


}

void CGameFramework::OnDestroy()
{
}



void CGameFramework::BuildObjects()
{
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

	ProcessInput();
	AnimateObjects();

	m_CommandQueue->Prepare_Rendering();
	
	// TODO : ������ �ڵ�� ���⿡ �߰��� ���̴�. 
	if (m_pTestObj)
		m_pTestObj->Render();


	m_CommandQueue->Execute_Rendering();


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
