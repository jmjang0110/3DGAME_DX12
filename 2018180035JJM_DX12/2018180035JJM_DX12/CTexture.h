#pragma once


class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

private:
	UINT										m_nTextureType;

/// TEXTURES 
	int											m_nTextures = 0;
	std::vector<ComPtr<ID3D12Resource>>			m_ppd3dTextures{};
	std::vector<ComPtr<ID3D12Resource>>			m_ppd3dTextureUploadBuffers{};

	std::vector<UINT>							m_pnResourceTypes{};
	
/// DXGI BUFFER FORMAT ELEMENTS
	std::vector<_TCHAR*>						m_ppstrTextureNames{};
	std::vector<DXGI_FORMAT>					m_pdxgiBufferFormats{};
	std::vector<int>							m_pnBufferElements{};

/// ROOT PARAMETERS
	int											m_nRootParameters = 0;
	std::vector<int>							m_pnRootParameterIndices{};
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	m_pd3dSrvGpuDescriptorHandles{};

/// SAMPLER 
	int											m_nSamplers = 0;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>	m_pd3dSamplerGpuDescriptorHandles{};

public:
	void UpdateShaderVariable(int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables();

	void ReleaseShaderVariables();
	void ReleaseUploadBuffers();

	void LoadTextureFromDDSFile(wchar_t* pszFileName, UINT nResourceType, UINT nIndex);

/// [ S E T ] 
	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

/// [ G E T ] 
	int							GetRootParameters()					{ return(m_nRootParameters); }
	int							GetTextures()						{ return(m_nTextures); }
	_TCHAR*						GetTextureName(int nIndex)			{ return(m_ppstrTextureNames[nIndex]); }
	ID3D12Resource*				GetResource(int nIndex)				{ return(m_ppd3dTextures[nIndex].Get()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex)	{ return(m_pd3dSrvGpuDescriptorHandles[nIndex]); }
	int							GetRootParameter(int nIndex)		{ return(m_pnRootParameterIndices[nIndex]); }

	UINT						GetTextureType()					{ return(m_nTextureType); }
	UINT						GetTextureType(int nIndex)			{ return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT					GetBufferFormat(int nIndex)			{ return(m_pdxgiBufferFormats[nIndex]); }
	int							GetBufferElements(int nIndex)		{ return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);


};

