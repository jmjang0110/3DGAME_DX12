#pragma once

/*
	SwapChain
		- 현재 게임 세상에 있는 상황을 묘사
		- 어떤 공식으로 어떻게 계산할지 던져줌
		- GPU 가 계산
		- 결과물 받아서 화면에 그린다

		- 렌더링 결과물을 Buffer 에 그려서 건내달라고 부탁
		- 화면에 현재 결과물을 출력하는 와중에 다음 화면도 그려야 한다.
		- 현재 화면 결과물은 이미 화면 출력에 사용중으로 Double Buffering 방식으로
		- 하나는 화면을 그리고 다른 하나는 다음 화면을 그린다.

		[0] <--> [1]
*/


class CSwapChain
{

private:
	ComPtr<IDXGISwapChain3>		m_pdxgiSwapChain;	//스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다. 
	bool						m_bMsaa4xEnable        = false;
	UINT						m_nMsaa4xQualityLevels = 0;	//MSAA 다중 샘플링을 활성화하고 다중 샘플링 레벨을 설정한다.

	static const UINT			m_nSwapChainBuffers		= 2;//스왑 체인의 후면 버퍼의 개수이다. 
	UINT						m_nSwapChainBufferIndex;	//현재 스왑 체인의 후면 버퍼 인덱스이다.

private:
/// RTV
	ComPtr<ID3D12Resource>				m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];
	ComPtr<ID3D12DescriptorHeap>		m_pd3dRtvDescriptorHeap;
	UINT								m_nRtvDescriptorIncrementSize;//렌더 타겟 버퍼, 서술자 힙 인터페이스 포인터, 렌더 타겟 서술자 원소의 크기이다

/// DSV
	ComPtr<ID3D12Resource>				m_pd3dDepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap>		m_pd3dDsvDescriptorHeap;
	UINT								m_nDsvDescriptorIncrementSize;//깊이-스텐실 버퍼, 서술자 힙 인터페이스 포인터, 깊이-스텐실 서술자 원소의 크기이다. 

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

