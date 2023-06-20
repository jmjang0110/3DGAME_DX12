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
	D3D12_RECT					m_d3dScissorRect{};//뷰포트와 씨저 사각형이다. 

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
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); //프레임워크를 초기화하는 함수이다(주 윈도우가 생성되면 호출된다). 
	void OnDestroy();

	void BuildObjects();
	void ReleaseObjects();//렌더링할 메쉬와 게임 객체를 생성하고 소멸하는 함수이다. 
	
//프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수이다. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();
	void WaitForGpuComplete(); //CPU와 GPU를 동기화하는 함수이다. 


	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam,
		LPARAM lParam); //윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 


public:
	CScene* GetCurScene() { return m_pCurScene; }

};
