#include "stdafx.h"
#include "CCommandQueue.h"
#include "CGameFramework.h"
#include "CSwapChain.h"
#include "CDevice.h"


CCommandQueue::CCommandQueue()
{
}

CCommandQueue::~CCommandQueue()
{
}

void CCommandQueue::OnCreate(WindowInfo WindowInfo)
{
	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);

	if (pDevice.get()) return;

	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

// [ COMMAND QUEUE ]

	//����(Direct) ��� ť�� �����Ѵ�.
	HRESULT hResult = pDevice->GetDevice()->CreateCommandQueue(&d3dCommandQueueDesc,
		_uuidof(ID3D12CommandQueue), (void**)&m_pd3dCommandQueue);

// [ COMMAND ALLOCATOR ]

	//����(Direct) ��� �Ҵ��ڸ� �����Ѵ�. 
	hResult = pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
	__uuidof(ID3D12CommandAllocator), (void**)&m_pd3dCommandAllocator);

// [ COMMAND LIST ]

	//����(Direct) ��� ����Ʈ�� �����Ѵ�. 
	hResult = pDevice->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
	m_pd3dCommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void
		**)&m_pd3dCommandList);
	
	//��� ����Ʈ�� �����Ǹ� ����(Open) �����̹Ƿ� ����(Closed) ���·� �����
	hResult = m_pd3dCommandList->Close();

// [ FENCE ]

	HRESULT hResult = pDevice->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_pd3dFence);
	m_nFenceValue   = 0;//�潺�� �����ϰ� �潺 ���� 0���� �����Ѵ�. 
	m_hFenceEvent   = ::CreateEvent(NULL, FALSE, FALSE, NULL);





}

void CCommandQueue::Prepare_Rendering()
{
	std::shared_ptr<CSwapChain> pSwapChain = SWAP_CHAIN(CGameFramework);

	//��� �Ҵ��ڿ� ��� ����Ʈ�� �����Ѵ�.
	HRESULT hResult = m_pd3dCommandAllocator->Reset();
	hResult         = m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);
 

	/*	
	���� ���� Ÿ�ٿ� ���� ������Ʈ�� �����⸦ ��ٸ���. 
	������Ʈ�� ������ ���� Ÿ�� ������ ���´� 
	������Ʈ ���� (D3D12_RESOURCE_STATE_PRESENT)���� 
	���� Ÿ�� ����(D3D12_RESOURCE_STATE_RENDER_TARGET)�� �ٲ� ���̴�.
	*/
	::ZeroMemory(&m_d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));

	m_d3dResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_d3dResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_d3dResourceBarrier.Transition.pResource   = pSwapChain->GetCurRTVBackBuffer().Get();
	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	
	m_pd3dCommandList->ResourceBarrier(1, &m_d3dResourceBarrier);


	//����Ʈ�� ���� �簢���� �����Ѵ�.
	m_pd3dCommandList->RSSetViewports(1, &CGameFramework::GetInst()->GetViewPort());
	m_pd3dCommandList->RSSetScissorRects(1, &CGameFramework::GetInst()->GetScissorRect());
 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_nSwapChainBufferIndex * m_nRtvDescriptorIncrementSize);//������ ���� Ÿ�ٿ� �ش��ϴ� �������� CPU �ּ�(�ڵ�)�� ����Ѵ�. 
	

	//���ϴ� �������� ���� Ÿ��(��)�� �����. 
	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle,
		pfClearColor/*Colors::Azure*/, 0, NULL);
	
	//����-���ٽ� �������� CPU �ּҸ� ����Ѵ�. 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	
	//���ϴ� ������ ����-���ٽ�(��)�� �����. 
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle,D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	
	//���� Ÿ�� ��(������)�� ����-���ٽ� ��(������)�� ���-���� �ܰ�(OM)�� �����Ѵ�. 
	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE,
		&d3dDsvCPUDescriptorHandle);

}

void CCommandQueue::Execute_Rendering()
{

	/*
	���� ���� Ÿ�ٿ� ���� �������� �����⸦ ��ٸ���. 
	GPU�� ���� Ÿ��(����)�� �� �̻� ������� ������ 
	���� Ÿ���� ���´� ������Ʈ ����(D3D12_RESOURCE_STATE_PRESENT)�� �ٲ� ���̴�.
	*/

	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


	m_pd3dCommandList->ResourceBarrier(1, &m_d3dResourceBarrier);


	//��� ����Ʈ�� ���� ���·� �����.
	HRESULT hResult = m_pd3dCommandList->Close();


	//��� ����Ʈ�� ��� ť�� �߰��Ͽ� �����Ѵ�. - Command List ���� 
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList.Get()};
	//m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists); // ������ �ڵ� 
	m_pd3dCommandQueue->ExecuteCommandLists(_countof(ppd3dCommandLists), ppd3dCommandLists); // _countof ���  


	//GPU�� ��� ��� ����Ʈ�� ������ �� ���� ��ٸ���.
	WaitForGpuComplete();


	// Present 
	std::shared_ptr<CSwapChain> pSwapChain = SWAP_CHAIN(CGameFramework);
	pSwapChain->Present();
	pSwapChain->SwapIndex();



}

void CCommandQueue::WaitForGpuComplete()
{
	//CPU �潺�� ���� �����Ѵ�. 
	m_nFenceValue++;
	const UINT64 nFence = m_nFenceValue;
	
	//GPU�� �潺�� ���� �����ϴ� ����� ��� ť�� �߰��Ѵ�. 
	HRESULT hResult     = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFence);
	if (m_pd3dFence->GetCompletedValue() < nFence)
	{
		//�潺�� ���� ���� ������ ������ ������ �潺�� ���� ���� ������ ���� �� ������ ��ٸ���. 
		hResult = m_pd3dFence->SetEventOnCompletion(nFence, m_hFenceEvent);
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
