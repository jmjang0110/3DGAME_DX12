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
// 바인딩 ( binding ) : 리소스 객체를 쉐이더에 연결하는 과정 

/// [ 쉐이더 리소스 ]
/// - 렌더링 과정동안 GPU 가 사용하는 데이터 (비디오 메모리에 저장) 이다.
/// - 후면 버퍼 , 깊이/스텐실 버퍼, 텍스챠, 기하 데이터, 버퍼...
/// 
/// [ 리소스 뷰 ]
/// - 리소스는 쉐이더 파이프라인에 바인딩 ( 연결 ) 되어야 한다.
/// - 리소스는 파이프라인에 직접 연결되지 않고 서술자(뷰) 를 통해 연결한다.
/// - descriptor는 하나의 리소스에 대한 정보를 포함하고 있는 객체 ( 자료구조 ) 이다.
/// 
/// [ 서술자 ( Descriptor ) ]
/// - 리소스는 쉐이더 파이프라인에 바인딩 ( 연결 ) 되어야 사용할 수 있다.
/// - 서술자는 하나의 리소스에 대한 정보를 포함하고 있는 객체이다.
///   GPU 에게 리소스를 완전하게 설명(서술(하는 데이터 ( 리소스에 대한 가상 주소 포함 )
///   파이프라인이 리소스에 대한 접근을 하기 위해서 서술자를 참조해야 한다.
/// - 서술자는 서술자 테이블을 구성하도록 그룹화된다. 각 서술자 테이블은 하나의 유형의 리소스에 대한 정보를 저장한다.
/// - 서술자 테이블은 서술자 Heap 에 저장된다.

class CSwapChain
{

private:
	ComPtr<IDXGISwapChain3>		m_pdxgiSwapChain;					//스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다. 
	bool						m_bMsaa4xEnable				= false;
	UINT						m_nMsaa4xQualityLevels		= 0;	//MSAA 다중 샘플링을 활성화하고 다중 샘플링 레벨을 설정한다.

	UINT						m_BackBufferIdx;			//현재 스왑 체인의 후면 버퍼 인덱스이다.

	bool						m_bFullScreenState			= false;
private:
/// RTV
	ComPtr<ID3D12Resource>				m_ppd3dRenderTargetBuffers[SWAP_CHAIN_BUFFER_COUNT];
	D3D12_CPU_DESCRIPTOR_HANDLE			m__RTV_Handles[SWAP_CHAIN_BUFFER_COUNT];
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
	void OnCreate(WindowInfo WindowInfo);
	void Present();
	void ChangeSwapchainState();


public:
	void CreateRTV();
	void CreateDSV();

	void CreateRTVdescriptorHeap();
	void CreateDSVdescriptorHeap();



/// [ G E T ] 
public:
	ComPtr<IDXGISwapChain3>			GetDxgiSwapChain()		{ return m_pdxgiSwapChain; }
	ComPtr<ID3D12Resource>			GetRTVBuffer(UINT index);
	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTVHandle(UINT index);

	ComPtr<ID3D12Resource>			GetCurRTVBackBuffer();
	ComPtr<ID3D12DescriptorHeap>	GetRtvDescriptorHeap()  { return m_pd3dRtvDescriptorHeap; }
	UINT							GetRtvDescriptorIncrementSize() { return m_nRtvDescriptorIncrementSize; }
	UINT							GetCurBackBufferIndex() { return m_BackBufferIdx; }
	D3D12_CPU_DESCRIPTOR_HANDLE		GetCurRTVBackBufferHandle();

	D3D12_CPU_DESCRIPTOR_HANDLE		GetDSVHandle();
	bool							GetFullScreenState()	{ return m_bFullScreenState; }

/// [ S E T ]
public:
	void						SetFullScreenState(bool bFullScreenstate) { m_bFullScreenState = bFullScreenstate; }


};

