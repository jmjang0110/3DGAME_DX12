#include "stdafx.h"
#include "CSwapChain.h"
#include "CCommandQueue.h"
#include "CDevice.h"

#include "CGameFramework.h"

CSwapChain::CSwapChain()
{
}

CSwapChain::~CSwapChain()
{
}

void CSwapChain::OnCreate(WindowInfo windowInfo)
{
	RECT rcClient;
	::GetClientRect(windowInfo.hWnd, &rcClient);
	windowInfo.nWndClientWidth = rcClient.right - rcClient.left;
	windowInfo.nWndClientHeight = rcClient.bottom - rcClient.top;

	/// 4X MASS ǰ�� ���� ���� ���� 
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels{};
	d3dMsaaQualityLevels.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount      = 4;
	d3dMsaaQualityLevels.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	HRESULT hResult                               = DX12_DEVICE->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS
		, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels                = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable                       = (m_nMsaa4xQualityLevels > 1) ? true : false;


/// (1) Swapchain_Desc1
#ifdef Swapchain_Desc1
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc1{};
	::ZeroMemory(&dxgiSwapChainDesc1, sizeof(DXGI_SWAP_CHAIN_DESC1));

	dxgiSwapChainDesc1.Width				= windowInfo.nWndClientWidth;
	dxgiSwapChainDesc1.Height				= windowInfo.nWndClientHeight;
					 
	dxgiSwapChainDesc1.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc1.SampleDesc.Count		= (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc1.SampleDesc.Quality	= (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
					 
	dxgiSwapChainDesc1.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc1.BufferCount			= SWAP_CHAIN_BUFFER_COUNT;
					 
	dxgiSwapChainDesc1.Scaling				= DXGI_SCALING_NONE;
	dxgiSwapChainDesc1.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc1.AlphaMode			= DXGI_ALPHA_MODE_UNSPECIFIED;
	dxgiSwapChainDesc1.Flags				= 0;



/// (2) SWAP_CHAIN DESC - FULLSCREEN 
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));

	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator   = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed                = TRUE;
	
/// (3) MULTI SAMPLING SETTING 
///Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽��� �����Ѵ�.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;

	d3dMsaaQualityLevels.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount      = 4; //Msaa4x ���� ���ø�
	d3dMsaaQualityLevels.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;

	pDevice->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));

	m_nMsaa4xQualityLevels					= d3dMsaaQualityLevels.NumQualityLevels;		//����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ���Ѵ�. 
	m_bMsaa4xEnable							= (m_nMsaa4xQualityLevels > 1) ? true : false;	//���� ������ ǰ�� ������ 1���� ũ�� ���� ���ø��� Ȱ��ȭ�Ѵ�.


	pDevice->GetDxgiFactory4()->CreateSwapChainForHwnd(pCmdQueue->GetCommandQueue().Get(), windowInfo.hWnd,
		&dxgiSwapChainDesc1, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)m_pdxgiSwapChain.GetAddressOf());
	pDevice->GetDxgiFactory4()->MakeWindowAssociation(windowInfo.hWnd, DXGI_MWA_NO_ALT_ENTER);	//��Alt+Enter�� Ű�� ������ ��Ȱ��ȭ�Ѵ�. 
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();					//����ü���� ���� �ĸ���� �ε����� �����Ѵ�.

#endif

#define Swapchain_Desc
#ifdef Swapchain_Desc
/// (2) DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferDesc.Width                   = windowInfo.nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height                  = windowInfo.nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	
	dxgiSwapChainDesc.BufferCount                        = SWAP_CHAIN_BUFFER_COUNT;
	dxgiSwapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	dxgiSwapChainDesc.SampleDesc.Count                   = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality				 = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;

	dxgiSwapChainDesc.OutputWindow                       = windowInfo.hWnd;
	dxgiSwapChainDesc.Windowed							 = TRUE;
	dxgiSwapChainDesc.Flags								 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//��üȭ�� ��忡�� ����ȭ���� �ػ󵵸� ����ü��(�ĸ����)�� ũ�⿡ �°� �����Ѵ�. 
	dxgiSwapChainDesc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;



/// (4) CREATE SWPCHAIN
	hResult = DX12_DEVICE->GetDxgiFactory4()->CreateSwapChain(DX12_COMMAND_QUEUE->GetCommandQueue().Get()
		, &dxgiSwapChainDesc, (IDXGISwapChain**)m_pdxgiSwapChain.GetAddressOf());
	m_BackBufferIdx = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	hResult = DX12_DEVICE->GetDxgiFactory4()->MakeWindowAssociation(CGameFramework::GetInst()->GetWindowInfo().hWnd, DXGI_MWA_NO_ALT_ENTER);


#endif


	CreateRTVdescriptorHeap();
	CreateDSVdescriptorHeap();
#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRTV();
#endif
	CreateDSV();

}

void CSwapChain::Present()
{
/*
	����ü���� ������Ʈ�Ѵ�.
	������Ʈ�� �ϸ� ���� ���� Ÿ��(�ĸ����)�� ������
	������۷� �Ű����� ���� Ÿ�� �ε����� �ٲ� ���̴�.
*/

#ifdef _WITH_PRESENT_PARAMETERS
	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
#endif

#define _WITH_SYNCH_SWAPCHAIN
#ifdef _WITH_SYNCH_SWAPCHAIN
	m_pdxgiSwapChain->Present(1, 0);
#else

	m_pdxgiSwapChain->Present(0, 0);
#endif
}



void CSwapChain::ChangeSwapchainState()
{
/// CHANGE SWAP CHAIN STATE 
///	LabProject03 ������Ʈ�� ������� ��üȭ�� ������ ��ȯ�� �����Ѵ�.
/// ��F9���� ������ ��üȭ�� ����
///	��ȯ�ǰ� �ٽ� �� �� ������ �ٽ� ������ ���� ��ȯ�ǵ��� �Ѵ�.

	CGameFramework::GetInst()->WaitForGpuComplete();

	
	BOOL bFullScreenState = FALSE;
	m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
	m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);
	
	DXGI_MODE_DESC dxgiTargetParameters{};

	dxgiTargetParameters.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiTargetParameters.Width                   = CGameFramework::GetInst()->GetWindowInfo().nWndClientWidth;
	dxgiTargetParameters.Height                  = CGameFramework::GetInst()->GetWindowInfo().nWndClientHeight;
	dxgiTargetParameters.RefreshRate.Numerator   = 60;
	dxgiTargetParameters.RefreshRate.Denominator = 1;
	dxgiTargetParameters.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiTargetParameters.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	
	m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
		if (m_ppd3dRenderTargetBuffers[i])
			m_ppd3dRenderTargetBuffers[i]->Release();
	}
	
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, CGameFramework::GetInst()->GetWindowInfo().nWndClientWidth,
		CGameFramework::GetInst()->GetWindowInfo().nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
	
	m_BackBufferIdx = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

/// CREATE RENDER TARGET VIEWS 
	CreateRTV();


}



void CSwapChain::CreateRTV()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppd3dRenderTargetBuffers[i]));
		m__RTV_Handles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * m_nRtvDescriptorIncrementSize);
		DX12_DEVICE->GetDevice()->CreateRenderTargetView(m_ppd3dRenderTargetBuffers[i].Get(), nullptr, m__RTV_Handles[i]);
	}

}

void CSwapChain::CreateRTVdescriptorHeap()
{
	///���� Ÿ�� ������ ���� ������ ũ�⸦ �����Ѵ�. 
	m_nRtvDescriptorIncrementSize = DX12_DEVICE->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC RTVdesc;
	::ZeroMemory(&RTVdesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	RTVdesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	RTVdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVdesc.NodeMask = 0;

	///���� Ÿ�� ������ ��(�������� ������ ����ü�� ������ ����)�� �����Ѵ�.
	HRESULT hResult = DX12_DEVICE->GetDevice()->CreateDescriptorHeap(&RTVdesc,
		__uuidof(ID3D12DescriptorHeap), (void**)m_pd3dRtvDescriptorHeap.GetAddressOf());

}

void CSwapChain::CreateDSV()
{

	std::shared_ptr<CDevice> pDevice = DX12_DEVICE;
	WindowInfo sWindowInfo = CGameFramework::GetInst()->GetWindowInfo();

	D3D12_RESOURCE_DESC d3dResourceDesc{};
	d3dResourceDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment          = 0;
	d3dResourceDesc.Width              = sWindowInfo.nWndClientWidth;
	d3dResourceDesc.Height             = sWindowInfo.nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize   = 1;
	d3dResourceDesc.MipLevels          = 1;
	d3dResourceDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count   = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags              = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	
	
	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES)); 
	d3dHeapProperties.Type                     = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty          = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference     = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask         = 1;
	d3dHeapProperties.VisibleNodeMask          = 1;
	
	
	D3D12_CLEAR_VALUE d3dClearValue{};
	d3dClearValue.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth   = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;
	

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//����-���ٽ� ���۸� �����Ѵ�.
	pDevice->GetDevice()->CreateCommittedResource(&d3dHeapProperties
												, D3D12_HEAP_FLAG_NONE
												,&d3dResourceDesc
												, D3D12_RESOURCE_STATE_DEPTH_WRITE
												, &d3dClearValue
												,__uuidof(ID3D12Resource)
												, (void**)&m_pd3dDepthStencilBuffer);
	
	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc{};
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format        = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags         = D3D12_DSV_FLAG_NONE;

	pDevice->GetDevice()->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), &d3dDepthStencilViewDesc,
		d3dDsvCPUDescriptorHandle);

}

void CSwapChain::CreateDSVdescriptorHeap()
{
	std::shared_ptr<CDevice> pDevice = DX12_DEVICE;

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	int i = 0;

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	//����-���ٽ� ������ ��(�������� ������ 1)�� �����Ѵ�. 
	HRESULT hResult = pDevice->GetDevice()->CreateDescriptorHeap(&d3dDescriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

	//����-���ٽ� ������ ���� ������ ũ�⸦ �����Ѵ�.
	m_nDsvDescriptorIncrementSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

ComPtr<ID3D12Resource> CSwapChain::GetRTVBuffer(UINT index)
{
	if (index >= 0 && index < SWAP_CHAIN_BUFFER_COUNT) {
		return m_ppd3dRenderTargetBuffers[index];
	}
	return nullptr;

}

ComPtr<ID3D12Resource> CSwapChain::GetCurRTVBackBuffer()
{
	m_BackBufferIdx = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	return m_ppd3dRenderTargetBuffers[m_BackBufferIdx];

}

D3D12_CPU_DESCRIPTOR_HANDLE CSwapChain::GetRTVHandle(UINT index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (index * m_nRtvDescriptorIncrementSize); //������ ���� Ÿ�ٿ� �ش��ϴ� �������� CPU �ּ�(�ڵ�)�� ����Ѵ�. 
	return d3dRtvCPUDescriptorHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE CSwapChain::GetCurRTVBackBufferHandle()
{
	m_BackBufferIdx = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_BackBufferIdx * m_nRtvDescriptorIncrementSize); //������ ���� Ÿ�ٿ� �ش��ϴ� �������� CPU �ּ�(�ڵ�)�� ����Ѵ�. 
	return d3dRtvCPUDescriptorHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE CSwapChain::GetDSVHandle()
{
	///����-���ٽ� �������� CPU �ּҸ� ����Ѵ�. 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	return d3dDsvCPUDescriptorHandle;
}
