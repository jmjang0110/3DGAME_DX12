#pragma once

class CSwapChain;

/// [ Command Allocator ��� �Ҵ��� ]
/// 
/// GPU ��� ( Command )�� �����ϱ� ���� �޸�(����)�� �Ҵ�
/// D3D����̽��� ��� �Ҵ���( Command Allocator )���� ��ɸ���Ʈ( Command List ) �� �����Ѵ�.
/// Command List ( ��ɵ��� ���� )�� Command Allocator �� �޸𸮿� ����ȴ�. 
/// ���� Allocator���� ���ÿ� �� �� �̻��� Command List�� Open �� ���°� �� �� ����.

/// [ Command List ��� ����Ʈ ]
/// - GPU�� ������ ��ɵ��� ����ȭ�� ������ ��Ÿ���� ( ��ɵ��� ������� �����Ѵ�. )
/// - Command List�� �����Ǹ� ( OPen : ����� �߰��� �� �ִ� ) ���� �����̴�.
/// - Command List �� Close �ϸ� ����� �� �̻� �߰��� �� ����.
/// - ID3D12CommandList�� �Լ��� ȣ���ϴ� ���� Command Lust �� ����� �߰��ϴ� ���̴�.
/// - Command List ������ Command Allocator �� ������ ��ġ�ؾ��Ѵ�.
struct FrameContext
{
	ComPtr<ID3D12CommandAllocator>		pd3dCommandAllocator{};
	UINT64								FenceValue{};
};

class CCommandQueue
{
private:

	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList;	//��� ť, ��� �Ҵ���, ��� ����Ʈ �������̽� �������̴�. 
	//ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator;
	FrameContext						m_FrameContext[NUM_FRAMES_IN_FLIGHT];
	UINT								m_FrameIndex = 0;


	ComPtr<ID3D12Fence>					m_pd3dFence{};
	UINT64								m_nFenceValues[SWAP_CHAIN_BUFFER_COUNT];
	HANDLE								m_hFenceEvent = INVALID_HANDLE_VALUE;		//�潺 �������̽� ������, �潺�� ��, �̺�Ʈ �ڵ��̴�. 

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

