#pragma once

class CSwapChain;


class CCommandQueue
{
private:

	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue;
	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList;	//��� ť, ��� �Ҵ���, ��� ����Ʈ �������̽� �������̴�. 

	ComPtr<ID3D12Fence>					m_pd3dFence;
	UINT64								m_nFenceValue;
	HANDLE								m_hFenceEvent;		//�潺 �������̽� ������, �潺�� ��, �̺�Ʈ �ڵ��̴�. 

	D3D12_RESOURCE_BARRIER				m_d3dResourceBarrier;


private:
	std::shared_ptr<CSwapChain>			m_SwapChain;


public:
	CCommandQueue();
	~CCommandQueue();

public:
	void OnCreate(WindowInfo WindowInfo);

	void Prepare_Rendering();
	void Execute_Rendering();
	void WaitForGpuComplete();


	ComPtr<ID3D12CommandQueue> GetCommandQueue();
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();



};

