#include "stdafx.h"
#include "CDevice.h"

CDevice::CDevice()
{
}

CDevice::~CDevice()
{
}

void CDevice::OnCreate()
{
	HRESULT hResult;
	UINT nDXGIFactoryFlags = 0;

	
// [ DEBUG ]

#if defined(_DEBUG)
	ID3D12Debug* pd3dDebugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pd3dDebugController);
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

// [ DXGI FACTORY ]

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)m_pdxgiFactory.GetAddressOf());
	IDXGIAdapter1* pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&m_pd3dDevice))) break;
	}

// [ DEVICE ]

	//모든 하드웨어 어댑터 대하여 특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성한다. 
	if (!m_pd3dDevice)
	{
		m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)&pd3dAdapter);
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&m_pd3dDevice);
	}

	//씨저 사각형을 주 윈도우의 클라이언트 영역 전체로 설정한다.
	if (pd3dAdapter)
		pd3dAdapter->Release();
}

ComPtr<ID3D12Device> CDevice::GetDevice()
{
	return m_pd3dDevice;
}

ComPtr<IDXGIFactory4> CDevice::GetDxgiFactory4()
{
	return m_pdxgiFactory;
}
