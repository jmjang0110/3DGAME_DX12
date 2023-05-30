
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
	
	// TODO : 렌더링 코드는 여기에 추가될 것이다. 
	if (m_pTestObj)
		m_pTestObj->Render();


	m_CommandQueue->Execute_Rendering();


	/// RENDERING -> FULL SCREEN 터지네..? 
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
