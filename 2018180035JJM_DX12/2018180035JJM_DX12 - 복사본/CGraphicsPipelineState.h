#pragma once

class CShader;
class CGameObject;

class CGraphicsPipelineState 
{
private:
	std::vector<CGameObject*> m_vecGameObjects;


private:
/// SHADER 
	std::shared_ptr<CShader> m_VS = std::make_shared<CShader>();
	std::shared_ptr<CShader> m_PS = std::make_shared<CShader>();


public:
/// PIPELINE STSATE
	ComPtr<ID3D12PipelineState>			m_pd3dPipelineState{}; /// 그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_pipelineDesc{};

/// RASTERIZER / BLEND / INPUT LAYOUT / DEPTH STENCIL 
	D3D12_RASTERIZER_DESC				m_d3dRasterizerDesc{}; /// vertex shader의 output을 input으로 받는 두 번째 단계
	D3D12_BLEND_DESC					m_d3dBlendDesc{};
	D3D12_INPUT_LAYOUT_DESC				m_d3dInputLayoutDesc{};
	D3D12_DEPTH_STENCIL_DESC			m_d3dDepthStencilDesc{};


public:
	void OnCreate();

/// [ G E T ] 
	ComPtr<ID3D12PipelineState> GetPipelineState() { return m_pd3dPipelineState; }
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPipelineStateDesc() { return m_pipelineDesc; }


/// [ S E T ] 
	void SetVertexShader(std::shared_ptr<CShader> vs);
	void SetPixelShader(std::shared_ptr<CShader> ps);


	void SetRasterizerState(D3D12_RASTERIZER_DESC _RSdesc)			{ m_d3dRasterizerDesc   = _RSdesc;		}
	void SetBlendState(D3D12_BLEND_DESC _BlendDesc)					{ m_d3dBlendDesc        = _BlendDesc;	}
	void SetInputLayout(D3D12_INPUT_LAYOUT_DESC _ILdesc)			{ m_d3dInputLayoutDesc  = _ILdesc;		}
	void SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC _DSdesc)		{ m_d3dDepthStencilDesc = _DSdesc;		}

	HRESULT CreateGraphicsPipelineState();
	void SetPipelineState();
	void SetPipelineStateDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc) { m_pipelineDesc = desc; }


public:
	void PushBack_GameObject(CGameObject* pObj);
	void Clear_GameObject();

public:
	void ExecuteRender();

public:
	CGraphicsPipelineState();
	~CGraphicsPipelineState();


};