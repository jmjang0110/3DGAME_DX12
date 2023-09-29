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
// ���ε� ( binding ) : ���ҽ� ��ü�� ���̴��� �����ϴ� ���� 

/// [ ���̴� ���ҽ� ]
/// - ������ �������� GPU �� ����ϴ� ������ (���� �޸𸮿� ����) �̴�.
/// - �ĸ� ���� , ����/���ٽ� ����, �ؽ�í, ���� ������, ����...
/// 
/// [ ���ҽ� �� ]
/// - ���ҽ��� ���̴� ���������ο� ���ε� ( ���� ) �Ǿ�� �Ѵ�.
/// - ���ҽ��� ���������ο� ���� ������� �ʰ� ������(��) �� ���� �����Ѵ�.
/// - descriptor�� �ϳ��� ���ҽ��� ���� ������ �����ϰ� �ִ� ��ü ( �ڷᱸ�� ) �̴�.
/// 
/// [ ������ ( Descriptor ) ]
/// - ���ҽ��� ���̴� ���������ο� ���ε� ( ���� ) �Ǿ�� ����� �� �ִ�.
/// - �����ڴ� �ϳ��� ���ҽ��� ���� ������ �����ϰ� �ִ� ��ü�̴�.
///   GPU ���� ���ҽ��� �����ϰ� ����(����(�ϴ� ������ ( ���ҽ��� ���� ���� �ּ� ���� )
///   ������������ ���ҽ��� ���� ������ �ϱ� ���ؼ� �����ڸ� �����ؾ� �Ѵ�.
/// - �����ڴ� ������ ���̺��� �����ϵ��� �׷�ȭ�ȴ�. �� ������ ���̺��� �ϳ��� ������ ���ҽ��� ���� ������ �����Ѵ�.
/// - ������ ���̺��� ������ Heap �� ����ȴ�.

class CSwapChain
{

private:
	ComPtr<IDXGISwapChain3>		m_pdxgiSwapChain;					//���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ��ϴ�. 
	bool						m_bMsaa4xEnable				= false;
	UINT						m_nMsaa4xQualityLevels		= 0;	//MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.

	UINT						m_BackBufferIdx;			//���� ���� ü���� �ĸ� ���� �ε����̴�.

	bool						m_bFullScreenState			= false;
private:
/// RTV
	ComPtr<ID3D12Resource>				m_ppd3dRenderTargetBuffers[SWAP_CHAIN_BUFFER_COUNT];
	D3D12_CPU_DESCRIPTOR_HANDLE			m__RTV_Handles[SWAP_CHAIN_BUFFER_COUNT];
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

