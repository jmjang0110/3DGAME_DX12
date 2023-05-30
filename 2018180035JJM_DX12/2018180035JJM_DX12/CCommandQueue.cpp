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
	std::shared_ptr<CDevice> pDevice = DEVICE(CGameFramework);

	if (!pDevice.get()) 
		return;
	m_SwapChain = SWAP_CHAIN(CGameFramework);



	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

/// [ COMMAND QUEUE ]

	/// 직접(Direct) 명령 큐를 생성한다.
	HRESULT hResult = pDevice->GetDevice()->CreateCommandQueue(&d3dCommandQueueDesc,
		_uuidof(ID3D12CommandQueue), (void**)&m_pd3dCommandQueue);

/// [ COMMAND ALLOCATOR ]

	//직접(Direct) 명령 할당자를 생성한다. 
	hResult = pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
	__uuidof(ID3D12CommandAllocator), (void**)&m_pd3dCommandAllocator);

/// [ COMMAND LIST ]

	/// 직접(Direct) 명령 리스트를 생성한다. 
	hResult = pDevice->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
	m_pd3dCommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)m_pd3dCommandList.GetAddressOf());
	
	///명령 리스트는 생성되면 열린(Open) 상태이므로 닫힌(Closed) 상태로 만든다
	hResult = m_pd3dCommandList->Close();

/// [ FENCE ]

	hResult = pDevice->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)m_pd3dFence.GetAddressOf());
	for(int i = 0 ; i < SWAP_CHAIN_BUFFER_COUNT; ++i) 
		m_nFenceValues[i] = 0; /// 펜스를 생성하고 펜스 값을 0으로 설정한다. 

	m_hFenceEvent   = ::CreateEvent(NULL, FALSE, FALSE, NULL);

}

void CCommandQueue::Prepare_Rendering()
{

	///명령 할당자와 명령 리스트를 리셋한다.
	HRESULT hResult = m_pd3dCommandAllocator->Reset();
	hResult         = m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);
 

	/// 현재 렌더 타겟에 대한 프리젠트가 끝나기를 기다린다. 
	/// 프리젠트가 끝나면 렌더 타겟 버퍼의 상태는 
	/// 프리젠트 상태 (D3D12_RESOURCE_STATE_PRESENT)에서 
	/// 렌더 타겟 상태(D3D12_RESOURCE_STATE_RENDER_TARGET)로 바뀔 것이다.

	::ZeroMemory(&m_d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	m_d3dResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_d3dResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_d3dResourceBarrier.Transition.pResource   = SWAP_CHAIN(CGameFramework)->GetCurRTVBackBuffer().Get();
	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &m_d3dResourceBarrier);

	/// 그래픽 루트 시그너쳐를 설정한다. 
	COMMAND_QUEUE(CGameFramework)->GetCommandList()->SetGraphicsRootSignature(ROOT_SIGNATURE(CGameFramework)->GetRootSignature().Get());


	/// 뷰포트와 씨저 사각형을 설정한다.
	const D3D12_VIEWPORT viewPort = CGameFramework::GetInst()->GetViewPort();
	const D3D12_RECT ScrissorRect = CGameFramework::GetInst()->GetScissorRect();
	m_pd3dCommandList->RSSetViewports(1, &viewPort);
	m_pd3dCommandList->RSSetScissorRects(1, &ScrissorRect);
	
	/// 렌더 타겟 뷰(서술자)와 깊이-스텐실 뷰(서술자)를 출력-병합 단계(OM)에 연결한다. 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = SWAP_CHAIN(CGameFramework)->GetCurRTVBackBufferHandle();
	//d3dRtvCPUDescriptorHandle.ptr += (SWAP_CHAIN(CGameFramework)->GetCurBackBufferIndex() * SWAP_CHAIN(CGameFramework)->GetRtvDescriptorIncrementSize());
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = SWAP_CHAIN(CGameFramework)->GetDSVHandle();	 ///깊이-스텐실 서술자의 CPU 주소를 계산한다. 

	

	/// 원하는 색상으로 렌더 타겟(뷰)을 지운다. 
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, Colors::LightSkyBlue/* Clear Color */, 0, nullptr);
	/// 원하는 값으로 깊이-스텐실(뷰)을 지운다. 
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle,D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL); 

	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE,&d3dDsvCPUDescriptorHandle); /// 출력-병합 단계(OM)에 연결한다. 

}

void CCommandQueue::Execute_Rendering()
{

	
	/// 현재 렌더 타겟에 대한 렌더링이 끝나기를 기다린다. 
	/// GPU가 렌더 타겟(버퍼)을 더 이상 사용하지 않으면 
	/// 렌더 타겟의 상태는 프리젠트 상태(D3D12_RESOURCE_STATE_PRESENT)로 바뀔 것이다.


	m_d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_d3dResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
	m_d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


	m_pd3dCommandList->ResourceBarrier(1, &m_d3dResourceBarrier);
	/// 명령 리스트를 닫힌 상태로 만든다.
	HRESULT hResult = m_pd3dCommandList->Close();


	/// 명령 리스트를 명령 큐에 추가하여 실행한다. - Command List 수행 
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList.Get()};
m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists); // 교수님 코드 
	//m_pd3dCommandQueue->ExecuteCommandLists(_countof(ppd3dCommandLists),ppd3dCommandLists); // _countof 사용  

/// GPU가 모든 명령 리스트를 실행할 때 까지 기다린다.
	WaitForGpuComplete();

	/// Present 
	SWAP_CHAIN(CGameFramework)->Present();

	

	MoveToNextFrame();
	//SWAP_CHAIN(CGameFramework)->SwapIndex();
}

void CCommandQueue::WaitForGpuComplete()
{

/// CPU 펜스의 값을 증가한다. 
	UINT BackBufferIdx	= SWAP_CHAIN(CGameFramework)->GetDxgiSwapChain()->GetCurrentBackBufferIndex();
	UINT64 nFenceValue	= ++m_nFenceValues[BackBufferIdx];

/// GPU가 펜스의 값을 설정하는 명령을 명령 큐에 추가한다. 
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{

/// 펜스의 현재 값이 설정한 값보다 작으면 펜스의 현재 값이 설정한 값이 될 때까지 기다린다. 
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CCommandQueue::MoveToNextFrame()
{

/// MOVE TO NEXT FRAME 
	auto	 pSwapChain     = SWAP_CHAIN(CGameFramework);
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
