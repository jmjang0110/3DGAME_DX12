
#include "stdafx.h"
#include "CGameFramework.h"

#include "CDevice.h"
#include "CCommandQueue.h"
#include "CRootSignature.h"
#include "CSwapChain.h"


CGameFramework* CGameFramework::m_pInst = nullptr;

CGameFramework::CGameFramework()
{
	
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_Window.hInstance        = hInstance;
	m_Window.hWnd             = hMainWnd;
	m_Window.nWndClientHeight = FRAME_BUFFER_HEIGHT;
	m_Window.nWndClientWidth  = FRAME_BUFFER_WIDTH;

	//����Ʈ�� �� �������� Ŭ���̾�Ʈ ���� ��ü�� �����Ѵ�. 
	m_d3dScissorRect = { 0, 0, m_Window.nWndClientWidth, m_Window.nWndClientHeight };

	/*�潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� �����Ѵ�(�̺�Ʈ ��ü�� �ʱⰪ�� FALSE�̴�). �̺�Ʈ�� ����Ǹ�(Signal) ��
	��Ʈ�� ���� �ڵ������� FALSE�� �ǵ��� �����Ѵ�.*/
	m_d3dViewport.TopLeftX = 0;
	m_d3dViewport.TopLeftY = 0;
	m_d3dViewport.Width    = static_cast<float>(m_Window.nWndClientWidth);
	m_d3dViewport.Height   = static_cast<float>(m_Window.nWndClientHeight);
	m_d3dViewport.MinDepth = 0.0f;

	m_d3dViewport.MaxDepth = 1.0f;



	m_Device        = std::make_shared<CDevice>();
	m_CommandQueue  = std::make_shared<CCommandQueue>();
	m_SwapChain     = std::make_shared<CSwapChain>();
	m_RootSignature = std::make_shared<CRootSignature>();


// [ DEVICE ]
	m_Device->OnCreate();

// [ COMMAND QUEUE ]
	m_CommandQueue->OnCreate(m_Window);

// [ SWAPCHAIN ]
	m_SwapChain->OnCreate(m_Window);


	return true;


}

void CGameFramework::OnDestroy()
{
}

void CGameFramework::CreateSwapChain()
{
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
}

void CGameFramework::CreateDirect3DDevice()
{
}

void CGameFramework::CreateCommandQueueAndList()
{
}

void CGameFramework::CreateRenderTargetViews()
{
}

void CGameFramework::CreateDepthStencilView()
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

	m_CommandQueue->Execute_Rendering();

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
