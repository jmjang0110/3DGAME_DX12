#include "stdafx.h"
#include "CTexture.h"
#include "DDSTextureLoader12.h"
#include "CFileManager.h"

#include "CCommandQueue.h"
#include "CDevice.h"
#include "CGameFramework.h"


CTexture::CTexture(int nTextures, UINT nTextureType, int nSamplers, int nRootParameters)
{
	m_nTextureType = nTextureType;
	m_nTextures    = nTextures;

	if (m_nTextures > 0)
	{
		/// TEXTURES INITIALIZE 
		for (int i = 0; i < m_nTextures; ++i) {
			m_ppd3dTextureUploadBuffers.push_back(nullptr);
			m_ppd3dTextures.push_back(nullptr);

			_TCHAR* TextureName = new _TCHAR[64]{};
			TextureName[0] = NULL;
			m_ppstrTextureNames.push_back(TextureName);

			/// GPU DESCRIPTOR HANDLE INITIALIZE
			D3D12_GPU_DESCRIPTOR_HANDLE EmptyHandle{};
			EmptyHandle.ptr = NULL;
			m_pd3dSrvGpuDescriptorHandles.push_back(EmptyHandle);
			
			m_pnResourceTypes.push_back(0);
			
			DXGI_FORMAT EmptyDXGI_Format{};
			m_pdxgiBufferFormats.push_back(EmptyDXGI_Format);

			m_pnBufferElements.push_back(0);
		}
	}
	m_nRootParameters = nRootParameters;
	for (int i = 0; i < m_nRootParameters; i++) {
		m_pnRootParameterIndices.push_back(-1);
	}
	m_nSamplers = nSamplers;
	if (m_nSamplers > 0) {
		D3D12_GPU_DESCRIPTOR_HANDLE emptyGPUdescHandle{};
		m_pd3dSamplerGpuDescriptorHandles.push_back(emptyGPUdescHandle);

	}
}

CTexture::~CTexture()
{
	for (int i = 0; i < m_nTextures; ++i)
	{
		m_ppstrTextureNames[i][0] = NULL;
		delete[] m_ppstrTextureNames[i];
	}


}

void CTexture::SetRootParameterIndex(int nIndex, UINT nRootParameterIndex)
{
	m_pnRootParameterIndices[nIndex] = nRootParameterIndex;
}

void CTexture::SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pd3dSrvGpuDescriptorHandles[nIndex] = d3dSrvGpuDescriptorHandle;
}

void CTexture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void CTexture::UpdateShaderVariables()
{
	if (m_nRootParameters == m_nTextures)
	{
		for (int i = 0; i < m_nRootParameters; i++)
		{
			if (m_pd3dSrvGpuDescriptorHandles[i].ptr && (m_pnRootParameterIndices[i] != -1)) 
				DX12_COMMAND_LIST->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[i], m_pd3dSrvGpuDescriptorHandles[i]);
		}
	}
	else
	{
		if (m_pd3dSrvGpuDescriptorHandles[0].ptr)
			DX12_COMMAND_LIST->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[0], m_pd3dSrvGpuDescriptorHandles[0]);
	}
}

void CTexture::UpdateShaderVariable(int nParameterIndex, int nTextureIndex)
{
	DX12_COMMAND_LIST->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[nParameterIndex], m_pd3dSrvGpuDescriptorHandles[nTextureIndex]);
}

void CTexture::ReleaseShaderVariables()
{
}

void CTexture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers.empty() == false)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		m_ppd3dTextureUploadBuffers.clear();
	}
}

void CTexture::LoadTextureFromDDSFile(wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = FILE_MGR->CreateTextureResourceFromDDSFile(pszFileName
																				, &m_ppd3dTextureUploadBuffers[nIndex]
																				, D3D12_RESOURCE_STATE_GENERIC_READ/*D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE*/);
}

D3D12_SHADER_RESOURCE_VIEW_DESC CTexture::GetShaderResourceViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = GetResource(nIndex);
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc{};
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	int nTextureType = GetTextureType(nIndex);
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY: //[]
		d3dShaderResourceViewDesc.Format                        = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels           = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip     = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice          = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.Format                             = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels           = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip     = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice          = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice     = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize           = d3dResourceDesc.DepthOrArraySize;
		break;
	case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.Format                          = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels           = 1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip     = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.Format                     = m_pdxgiBufferFormats[nIndex];
		d3dShaderResourceViewDesc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement        = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements         = m_pnBufferElements[nIndex];
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}