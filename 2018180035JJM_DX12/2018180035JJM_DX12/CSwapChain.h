#pragma once

/*
	SwapChain
		- ���� ���� ���� �ִ� ��Ȳ�� ����
		- � �������� ��� ������� ������
		- GPU �� ���
		- ����� �޾Ƽ� ȭ�鿡 �׸���

		- ������ ������� Buffer �� �׷��� �ǳ��޶�� ��Ź
		- ȭ�鿡 ���� ������� ����ϴ� ���߿� ���� ȭ�鵵 �׷��� �Ѵ�.
		- ���� ȭ�� ������� �̹� ȭ�� ��¿� ��������� Double Buffering �������
		- �ϳ��� ȭ���� �׸��� �ٸ� �ϳ��� ���� ȭ���� �׸���.

		[0] <--> [1]
*/


class CSwapChain
{

private:
	ComPtr<IDXGISwapChain3>		m_pdxgiSwapChain;	//���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ��ϴ�. 
	bool						m_bMsaa4xEnable        = false;
	UINT						m_nMsaa4xQualityLevels = 0;	//MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.

	static const UINT			m_nSwapChainBuffers		= 2;//���� ü���� �ĸ� ������ �����̴�. 
	UINT						m_nSwapChainBufferIndex;	//���� ���� ü���� �ĸ� ���� �ε����̴�.

private:
/// RTV
	ComPtr<ID3D12Resource>				m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];
	ComPtr<ID3D12DescriptorHeap>		m_pd3dRtvDescriptorHeap;
	UINT								m_nRtvDescriptorIncrementSize;//���� Ÿ�� ����, ������ �� �������̽� ������, ���� Ÿ�� ������ ������ ũ���̴�

/// DSV
	ComPtr<ID3D12Resource>				m_pd3dDepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap>		m_pd3dDsvDescriptorHeap;
	UINT								m_nDsvDescriptorIncrementSize;//����-���ٽ� ����, ������ �� �������̽� ������, ����-���ٽ� ������ ������ ũ���̴�. 

public:
	CSwapChain();
	~CSwapChain();


public:
	ComPtr<IDXGISwapChain3> GetDxgiSwapChain() { return m_pdxgiSwapChain; }


public:
	void OnCreate(WindowInfo WindowInfo);
	void Present();
	void SwapIndex();

	ComPtr<ID3D12Resource> GetRTVBuffer(UINT index);
	ComPtr<ID3D12Resource> GetCurRTVBackBuffer();


public:
	void CreateRTV();
	void CreateDSV();

	void CreateRTVdescriptorHeap();
	void CreateDSVdescriptorHeap();

public:
	void SetSwapchainDesc(WindowInfo windowInfo
		, DXGI_SWAP_CHAIN_DESC1* dxgiSwapChainDesc);

	void SetSwapchainDesc_FullScreen(WindowInfo windowInfo
		, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* dxgiSwapChainDesc);

	void SetMsaaQualityLevels(WindowInfo windowInfo,
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS* d3dMsaaQualityLevels);
};

