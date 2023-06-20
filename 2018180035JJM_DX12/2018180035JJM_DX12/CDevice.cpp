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
	ComPtr<ID3D12Debug> pd3dDebugController = nullptr;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)pd3dDebugController.GetAddressOf());
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		//pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

// [ DXGI FACTORY ]

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void**)m_pdxgiFactory.GetAddressOf());
	ComPtr<IDXGIAdapter1> pd3dAdapter = nullptr;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, pd3dAdapter.GetAddressOf()); i++)
	{

		DXGI_ADAPTER_DESC1 dxgiAdapterDesc{};
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;
		
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)m_pd3dDevice.GetAddressOf())))
			break;
	}


// [ DEVICE ]

	//모든 하드웨어 어댑터 대하여 특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성한다. 
	if (!(pd3dAdapter.Get()))
	{
		m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void**)pd3dAdapter.GetAddressOf());
		hResult = D3D12CreateDevice(pd3dAdapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)m_pd3dDevice.GetAddressOf());
	}


}

ComPtr<ID3D12Device> CDevice::GetDevice()
{
	return m_pd3dDevice;
}

ComPtr<IDXGIFactory4> CDevice::GetDxgiFactory4()
{
	return m_pdxgiFactory;
}
