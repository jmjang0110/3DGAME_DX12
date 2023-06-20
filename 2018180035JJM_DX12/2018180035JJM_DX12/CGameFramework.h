#pragma once

class CDevice;
class CCommandQueue;
class CShader;
class CRootSignature;
class CSwapChain;
class CGraphicsPipelineState;

class CScene;


class CGameFramework
{
	SINGLETON_PATTERN(CGameFramework);

private:
	CScene* m_pCurScene{};


private:
// WINDOW 
	WindowInfo					m_Window{};
	D3D12_VIEWPORT				m_d3dViewport{};
	D3D12_RECT					m_d3dScissorRect{};//����Ʈ�� ���� �簢���̴�. 

// DXGI DEVICE 
	std::shared_ptr<CDevice>					m_Device{};
	std::shared_ptr<CCommandQueue>				m_CommandQueue{};
	std::shared_ptr<CSwapChain>					m_SwapChain{};
	std::shared_ptr<CRootSignature>				m_RootSignature{};
	std::shared_ptr<CGraphicsPipelineState>		m_GraphicsPipelineStateMachine{};

	_TCHAR						m_pszFrameRate[50];

public:
// [ GET ]
	std::shared_ptr<class CDevice>					GetDevice()									{ return m_Device; }
	std::shared_ptr<class CCommandQueue>			GetCmdQueue()								{ return m_CommandQueue; }
	std::shared_ptr<class CSwapChain>				GetSwapChain()								{ return m_SwapChain; }
	std::shared_ptr<class CRootSignature>			GetRootSignature()							{ return m_RootSignature; }
	std::shared_ptr<class CGraphicsPipelineState>	GetGraphicsPipelineStateMachine()			{ return m_GraphicsPipelineStateMachine; }


	WindowInfo								GetWindowInfo()			{ return m_Window; }
	D3D12_VIEWPORT							GetViewPort()			{ return m_d3dViewport; }
	D3D12_RECT								GetScissorRect()		{ return m_d3dScissorRect; }

public:
	CGameFramework();
	~CGameFramework();

public:
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); //�����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��̴�(�� �����찡 �����Ǹ� ȣ��ȴ�). 
	void OnDestroy();

	void BuildObjects();
	void ReleaseObjects();//�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�. 
	
//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();
	void WaitForGpuComplete(); //CPU�� GPU�� ����ȭ�ϴ� �Լ��̴�. 


	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam,
		LPARAM lParam); //�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 


public:
	CScene* GetCurScene() { return m_pCurScene; }

};
