#include "stdafx.h"
#include "CCommandQueue.h"
#include "CGameFramework.h"
#include "CSwapChain.h"
#include "CDevice.h"
#include "CRootSignature.h"


CCommandQueue::CCommandQueue()
{
}

CCommandQueue::~CCommandQueue()
{
}

void CCommandQueue::OnCreate(WindowInfo WindowInfo)
{
	std::shared_ptr<CDevice> pDevice = DX12_DEVICE;

	if (!pDevice.get()) 
		return;
	



	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

	d3dCommandQueueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
	d3dCommandQueueDesc.NodeMask = 1;

/// [ COMMAND QUEUE ]

	/// ����(Direct) ��� ť�� �����Ѵ�.
	HRESULT hResult = pDevice->GetDevice()->CreateCommandQueue(&d3dCommandQueueDesc,
		_uuidof(ID3D12CommandQueue), (void**)&m_pd3dCommandQueue);

/// [ COMMAND ALLOCATOR ]

	//����(Direct) ��� �Ҵ��ڸ� �����Ѵ�. 
	for (int i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
	{
		//hResult = pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		//__uuidof(ID3D12CommandAllocator), (void**)&m_pd3dCommandAllocator);
		hResult = pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			__uuidof(ID3D12CommandAllocator), (void**)&m_FrameContext[i].pd3dCommandAllocator);

	}

/// [ COMMAND LIST ]

	/// ����(Direct) ��� ����Ʈ�� �����Ѵ�. 
	hResult = pDevice->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
	m_FrameContext[0].pd3dCommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)m_pd3dCommandList.GetAddressOf());
	
	///��� ����Ʈ�� �����Ǹ� ����(Open) �����̹Ƿ� ����(Closed) ���·� �����
	hResult = m_pd3dCommandList->Close();

/// [ FENCE ]

	hResult = pDevice->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE
, __uuidof(ID3D12Fence), (void**)m_pd3dFence.GetAddressOf());
	for(int i = 0 ; i < SWAP_CHAIN_BUFFER_COUNT; ++i) 
		m_nFenceValues[i] = 0; /// �潺�� �����ϰ� �潺 ���� 0���� �����Ѵ�. 

	m_hFenceEvent   = ::CreateEvent(NULL, FALSE, FALSE, NULL);

}

void CCommandQueue::Prepare_Rendering()
{

	///��� �Ҵ��ڿ� ��� ����Ʈ�� �����Ѵ�.
	//HRESULT hResult = m_pd3dCommandAllocator->Reset();

/// * pd3dCommandAllocator->Reset()
/// 	-> ��� ��Ͽ� ���õ� �޸��� ��Ȱ���� ���� ��� �Ҵ��ڸ� �缳���Ѵ�. 
///	       �缳���� GPU�� ���� ��� ��ϵ��� ��� ó���� �Ŀ� �Ͼ��.
/// 
/// * m_pd3dCommandList->Reset
///		->	��� ����� ExecuteCommandList �� ���ؼ� ��� ��⿭�� �߰��ߴٸ� ��� ����� �缳���� �� �ִ�.
///			��� ����� �缳���ϸ� �޸𸮰� ��Ȱ�� �ȴ�.

	HRESULT hResult = m_FrameContext[DX12_SWAP_CHAIN->GetDxgiSwapChain()->GetCurrentBackBufferIndex()].pd3dCommandAllocator->Reset();
	hResult         = m_pd3dCommandList->Reset(m_FrameContext[DX12_SWAP_CHAIN->GetDxgiSwapChain()->GetCurrentBackBufferIndex()].pd3dCommandAllocator.Get(), NULL);
 

	/// ���� ���� Ÿ�ٿ� ���� ������Ʈ�� �����⸦ ��ٸ���. 
	/// ������Ʈ�� ������ ���� Ÿ�� ������ ���´� 
	/// ������Ʈ ���� (D3D12_RESOURCE_STATE_PRESENT)���� 
	/// ���� Ÿ�� ����(D3D12_RESOURCE_STATE_RENDER_TARGET)�� �ٲ� ���̴�.

	::ZeroMemory(&m_d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	m_d3dResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_d3dResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_d3dResourceBarrier.Transition.pResource   = DX12_SWAP_CHAIN->GetCurRTVBackBuffer().Get();
	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	DX12_COMMAND_LIST->ResourceBarrier(1, &m_d3dResourceBarrier);  /// �ڿ� �뵵�� ���õ� ���� ���̸� Direct3D �� �����Ѵ�. 

	/// �׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�. 
	DX12_COMMAND_LIST->SetGraphicsRootSignature(DX12_ROOT_SIGNATURE.Get());


	/// ����Ʈ�� ���� �簢���� �����Ѵ�.
	const D3D12_VIEWPORT viewPort = CGameFramework::GetInst()->GetViewPort();
	const D3D12_RECT ScrissorRect = CGameFramework::GetInst()->GetScissorRect();
	DX12_COMMAND_LIST->RSSetViewports(1, &viewPort);
	DX12_COMMAND_LIST->RSSetScissorRects(1, &ScrissorRect);
	
	/// ���� Ÿ�� ��(������)�� ����-���ٽ� ��(������)�� ���-���� �ܰ�(OM)�� �����Ѵ�. 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = DX12_SWAP_CHAIN->GetCurRTVBackBufferHandle();
	//d3dRtvCPUDescriptorHandle.ptr += (DX12_SWAP_CHAIN->GetCurBackBufferIndex() * DX12_SWAP_CHAIN->GetRtvDescriptorIncrementSize());
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = DX12_SWAP_CHAIN->GetDSVHandle();	 ///����-���ٽ� �������� CPU �ּҸ� ����Ѵ�. 

	

	/// ���ϴ� �������� ���� Ÿ��(��)�� �����. 
	DX12_COMMAND_LIST->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, Colors::LightSkyBlue/* Clear Color */, 0, nullptr);
	/// ���ϴ� ������ ����-���ٽ�(��)�� �����. 
	DX12_COMMAND_LIST->ClearDepthStencilView(d3dDsvCPUDescriptorHandle,D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	/// ������ ����� ��ϵ� ����Ÿ�ٹ��۵��� �����Ѵ�. 
	DX12_COMMAND_LIST->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle); /// ���-���� �ܰ�(OM)�� �����Ѵ�. 

}

void CCommandQueue::Execute_Rendering()
{

	
	/// ���� ���� Ÿ�ٿ� ���� �������� �����⸦ ��ٸ���. 
	/// GPU�� ���� Ÿ��(����)�� �� �̻� ������� ������ 
	/// ���� Ÿ���� ���´� ������Ʈ ����(D3D12_RESOURCE_STATE_PRESENT)�� �ٲ� ���̴�.


	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;



	DX12_COMMAND_LIST->ResourceBarrier(1, &m_d3dResourceBarrier);
	/// ��� ����Ʈ�� ���� ���·� �����. ( ��ɵ��� ����� ��ģ�� ) 
	HRESULT hResult = DX12_COMMAND_LIST->Close();


	/// ��� ����Ʈ�� ��� ť�� �߰��Ͽ� �����Ѵ�. - Command List ���� 
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList.Get()};
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists); // ������ �ڵ� 
	//m_pd3dCommandQueue->ExecuteCommandLists(_countof(ppd3dCommandLists),ppd3dCommandLists); // _countof ���  

/// GPU�� ��� ��� ����Ʈ�� ������ �� ���� ��ٸ���.
	WaitForGpuComplete();

	/// Present 
	DX12_SWAP_CHAIN->Present();

	

	MoveToNextFrame();
	//DX12_SWAP_CHAIN->SwapIndex();
}

void CCommandQueue::WaitForGpuComplete()
{

/// CPU �潺�� ���� �����Ѵ�. 
	UINT BackBufferIdx	= DX12_SWAP_CHAIN->GetDxgiSwapChain()->GetCurrentBackBufferIndex();
	UINT64 nFenceValue	= ++m_nFenceValues[BackBufferIdx];

/// GPU�� �潺�� ���� �����ϴ� ����� ��� ť�� �߰��Ѵ�. 
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{

/// �潺�� ���� ���� ������ ������ ������ �潺�� ���� ���� ������ ���� �� ������ ��ٸ���. 
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CCommandQueue::MoveToNextFrame()
{

/// MOVE TO NEXT FRAME 
	auto	 pSwapChain     = DX12_SWAP_CHAIN;
	UINT	 BackBufferIdx  = pSwapChain->GetDxgiSwapChain()->GetCurrentBackBufferIndex();
	UINT64	 nFenceValue    = ++m_nFenceValues[BackBufferIdx];
	HRESULT  hResult        = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}


}


ComPtr<ID3D12CommandQueue> CCommandQueue::GetCommandQueue()
{
	return m_pd3dCommandQueue;
}

ComPtr<ID3D12GraphicsCommandList> CCommandQueue::GetCommandList()
{
	return m_pd3dCommandList;
}

ComPtr<ID3D12CommandAllocator> CCommandQueue::GetCommandAllocator()
{
	return m_FrameContext[DX12_SWAP_CHAIN->GetDxgiSwapChain()->GetCurrentBackBufferIndex()].pd3dCommandAllocator;

	//return m_pd3dCommandAllocator;

}
