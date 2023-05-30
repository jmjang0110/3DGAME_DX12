#pragma once

class CShader;

class CGraphicsPipelineState 
{
private:
/// SHADER 
	std::shared_ptr<CShader> m_VS{};
	std::shared_ptr<CShader> m_PS{};


public:
/// PIPELINE STSATE
	ComPtr<ID3D12PipelineState>			m_pd3dPipelineState{}; //�׷��Ƚ� ���������� ���� ��ü�� ���� �������̽� ������
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_pipelineDesc{};

/// RASTERIZER / BLEND 
	D3D12_RASTERIZER_DESC				m_d3dRasterizerDesc{};
	D3D12_BLEND_DESC					m_d3dBlendDesc{};

public:
	void OnCreate();


	ComPtr<ID3D12PipelineState> GetPipelineState() { return m_pd3dPipelineState; }

	void SetVertexShader(std::shared_ptr<CShader> vs);
	void SetPixelShader(std::shared_ptr<CShader> ps);

	void SetRasterizerState();
	void SetBlendState();
	void SetGraphicsPipelineState();


public:
	CGraphicsPipelineState();
	~CGraphicsPipelineState();


};