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


// (1) SWAPCHAIN DESC 
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc{};
	SetSwapchainDesc(windowInfo, &dxgiSwapChainDesc);

// (2) SWPCHAIN DESC - FULLSCREEN 
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	SetSwapchainDesc_FullScreen(windowInfo, &dxgiSwapChainFullScreenDesc);
	
// (3) MULTI SAMPLING SETTING 
	//Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽��� �����Ѵ�.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	SetMsaaQualityLevels(windowInfo, &d3dMsaaQualityLevels);

// (4) CREATE SWPCHAIN
	std::shared_ptr<CDevice>		pDevice          = DEVICE(CGameFramework);
	std::shared_ptr<CCommandQueue>  pCmdQueue        = COMMAND_QUEUE(CGameFramework);

	pDevice->GetDxgiFactory4()->CreateSwapChainForHwnd(pCmdQueue->GetCommandQueue().Get(), windowInfo.hWnd,
		&dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)m_pdxgiSwapChain.Get());

	pDevice->GetDxgiFactory4()->MakeWindowAssociation(windowInfo.hWnd , DXGI_MWA_NO_ALT_ENTER);	//��Alt+Enter�� Ű�� ������ ��Ȱ��ȭ�Ѵ�. 
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();			//����ü���� ���� �ĸ���� �ε����� �����Ѵ�. 
		

}

void CSwapChain::Present()
{
	/*
	����ü���� ������Ʈ�Ѵ�.
	������Ʈ�� �ϸ� ���� ���� Ÿ��(�ĸ����)�� ������
	������۷� �Ű����� ���� Ÿ�� �ε����� �ٲ� ���̴�.
*/

	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;

	std::shared_ptr<CSwapChain> pSwapChain = SWAP_CHAIN(CGameFramework);
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);

}

void CSwapChain::SwapIndex()
{

	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

}

ComPtr<ID3D12Resource> CSwapChain::GetRTVBuffer(UINT index)
{
	if (index >= 0 && index < m_nSwapChainBuffers) {
		return m_ppd3dRenderTargetBuffers[index];
	}
	return nullptr;

}

ComPtr<ID3D12Resource> CSwapChain::GetCurRTVBackBuffer()
{
	
	return m_ppd3dRenderTargetBuffers[m_nSwapChainBufferIndex];

}

void CSwapChain::CreateRTVdescriptorHeap()
{
	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers;
	d3dDescriptorHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; 
	d3dDescriptorHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask       = 0;

	//���� Ÿ�� ������ ��(�������� ������ ����ü�� ������ ����)�� �����Ѵ�.
	HRESULT hResult                      = pDevice->GetDevice()->CreateDescriptorHeap(&d3dDescriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dRtvDescriptorHeap);

	//���� Ÿ�� ������ ���� ������ ũ�⸦ �����Ѵ�. 
	m_nRtvDescriptorIncrementSize = 
		pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void CSwapChain::CreateDSVdescriptorHeap()
{
	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDescriptorHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask       = 0;

	//����-���ٽ� ������ ��(�������� ������ 1)�� �����Ѵ�. 
	HRESULT hResult = pDevice->GetDevice()->CreateDescriptorHeap(&d3dDescriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);
	
	//����-���ٽ� ������ ���� ������ ũ�⸦ �����Ѵ�.
	m_nDsvDescriptorIncrementSize = 
		pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void CSwapChain::CreateRTV()
{

	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);


	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle =
		m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < m_nSwapChainBuffers; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, __uuidof(ID3D12Resource)
			, (void**)&m_ppd3dRenderTargetBuffers[i]);

		pDevice->GetDevice()->CreateRenderTargetView(m_ppd3dRenderTargetBuffers[i].Get()
			, NULL,d3dRtvCPUDescriptorHandle);

		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}
}

void CSwapChain::CreateDSV()
{

	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);
	WindowInfo sWindowInfo = CGameFramework::GetInst()->GetWindowInfo();

	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment          = 0;
	d3dResourceDesc.Width              = sWindowInfo.nWndClientWidth;
	d3dResourceDesc.Height             = sWindowInfo.nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize   = 1;
	d3dResourceDesc.MipLevels          = 1;
	d3dResourceDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count   = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	
	
	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES)); 
	d3dHeapProperties.Type                     = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty          = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference     = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask         = 1;
	d3dHeapProperties.VisibleNodeMask          = 1;
	
	
	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth   = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;
	
	//����-���ٽ� ���۸� �����Ѵ�.
	pDevice->GetDevice()->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE,
		&d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue,
		__uuidof(ID3D12Resource), (void**)&m_pd3dDepthStencilBuffer);
	

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = 
	m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	
	//����-���ٽ� ���� �並 �����Ѵ�.
	pDevice->GetDevice()->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), NULL,
		d3dDsvCPUDescriptorHandle);

}

void CSwapChain::SetSwapchainDesc(WindowInfo windowInfo
	, DXGI_SWAP_CHAIN_DESC1* dxgiSwapChainDesc)
{
	::ZeroMemory(dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));

	dxgiSwapChainDesc->Width				= windowInfo.nWndClientWidth;
	dxgiSwapChainDesc->Height				= windowInfo.nWndClientHeight;

	dxgiSwapChainDesc->Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc->SampleDesc.Count		= (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc->SampleDesc.Quality	= (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;

	dxgiSwapChainDesc->BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc->BufferCount			= m_nSwapChainBuffers;

	dxgiSwapChainDesc->Scaling				= DXGI_SCALING_NONE;
	dxgiSwapChainDesc->SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc->AlphaMode			= DXGI_ALPHA_MODE_UNSPECIFIED;
	dxgiSwapChainDesc->Flags				= 0;
}

void CSwapChain::SetSwapchainDesc_FullScreen(WindowInfo windowInfo
	, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* dxgiSwapChainFullScreenDesc)
{
	::ZeroMemory(dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));

	dxgiSwapChainFullScreenDesc->RefreshRate.Numerator   = 60;
	dxgiSwapChainFullScreenDesc->RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc->ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc->Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc->Windowed                = TRUE;
}

void CSwapChain::SetMsaaQualityLevels(WindowInfo windowInfo
	, D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS* d3dMsaaQualityLevels)
{
	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);

	d3dMsaaQualityLevels->Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels->SampleCount      = 4; //Msaa4x ���� ���ø�
	d3dMsaaQualityLevels->Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels->NumQualityLevels = 0;

	pDevice->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));

	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels->NumQualityLevels;	//����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ���Ѵ�. 
	m_bMsaa4xEnable        = (m_nMsaa4xQualityLevels > 1) ? true : false;	//���� ������ ǰ�� ������ 1���� ũ�� ���� ���ø��� Ȱ��ȭ�Ѵ�.
}

