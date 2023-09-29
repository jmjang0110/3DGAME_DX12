#pragma once

class CSwapChain;

/// [ Command Allocator 명령 할당자 ]
/// 
/// GPU 명령 ( Command )을 저장하기 위한 메모리(버퍼)를 할당
/// D3D디바이스는 명령 할당자( Command Allocator )에서 명령리스트( Command List ) 를 생성한다.
/// Command List ( 명령들의 집합 )는 Command Allocator 의 메모리에 저장된다. 
/// 같은 Allocator에서 동시에 두 개 이상의 Command List가 Open 된 상태가 될 수 없다.

/// [ Command List 명령 리스트 ]
/// - GPU가 실행할 명령들의 순서화된 집합을 나타낸다 ( 명령들을 순서대로 실행한다. )
/// - Command List는 생성되면 ( OPen : 명령을 추가할 수 있는 ) 열린 상태이다.
/// - Command List 를 Close 하면 명령을 더 이상 추가할 수 없다.
/// - ID3D12CommandList의 함수를 호출하는 것은 Command Lust 에 명령을 추가하는 것이다.
/// - Command List 유형과 Command Allocator 의 유형이 일치해야한다.
struct FrameContext
{
	ComPtr<ID3D12CommandAllocator>		pd3dCommandAllocator{};
	UINT64								FenceValue{};
};

class CCommandQueue
{
private:

	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList;	//명령 큐, 명령 할당자, 명령 리스트 인터페이스 포인터이다. 
	//ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator;
	FrameContext						m_FrameContext[NUM_FRAMES_IN_FLIGHT];
	UINT								m_FrameIndex = 0;


	ComPtr<ID3D12Fence>					m_pd3dFence{};
	UINT64								m_nFenceValues[SWAP_CHAIN_BUFFER_COUNT];
	HANDLE								m_hFenceEvent = INVALID_HANDLE_VALUE;		//펜스 인터페이스 포인터, 펜스의 값, 이벤트 핸들이다. 

	D3D12_RESOURCE_BARRIER				m_d3dResourceBarrier;

public:
	CCommandQueue();
	~CCommandQueue();

public:
	void OnCreate(WindowInfo WindowInfo);

	void Prepare_Rendering();
	void Execute_Rendering();
	void WaitForGpuComplete();
	void MoveToNextFrame();



	ComPtr<ID3D12CommandQueue> GetCommandQueue();
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();


};

