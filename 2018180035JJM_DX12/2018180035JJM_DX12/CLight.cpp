#include "stdafx.h"
#include "CLight.h"
#include "ResourceManager.h"

#include "CGameFramework.h"
#include "CCommandQueue.h"
#include "CDevice.h"
bool CLight::OnCreate()
{
	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	std::shared_ptr<LIGHT> pLight = std::make_shared<LIGHT>();
	pLight->m_bEnable         = true;
	pLight->m_nType           = POINT_LIGHT;
	pLight->m_fRange          = 1000.0f;
	pLight->m_xmf4Ambient     = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	pLight->m_xmf4Diffuse     = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	pLight->m_xmf4Specular    = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	pLight->m_xmf3Position    = XMFLOAT3(30.0f, 30.0f, 30.0f);
	pLight->m_xmf3Direction   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pLight->m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);


	m_pLights.insert({"POINT_LIGHT", pLight});

	std::shared_ptr<LIGHT> pLight2 = std::make_shared<LIGHT>();
	pLight2->m_bEnable         = true;
	pLight2->m_nType           = SPOT_LIGHT;
	pLight2->m_fRange          = 500.0f;
	pLight2->m_xmf4Ambient     = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	pLight2->m_xmf4Diffuse     = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pLight2->m_xmf4Specular    = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	pLight2->m_xmf3Position    = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	pLight2->m_xmf3Direction   = XMFLOAT3(0.0f, 0.0f, 1.0f);
	pLight2->m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	pLight2->m_fFalloff        = 8.0f;
	pLight2->m_fPhi            = (float)cos(XMConvertToRadians(40.0f));
	pLight2->m_fTheta          = (float)cos(XMConvertToRadians(20.0f));

	m_pLights.insert({ "SPOT_LIGHT", pLight2 });


	std::shared_ptr<LIGHT> pLight3 = std::make_shared<LIGHT>();
	pLight3->m_bEnable = true;
	pLight3->m_nType = DIRECTIONAL_LIGHT;
	pLight3->m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pLight3->m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	pLight3->m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	pLight3->m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);

	m_pLights.insert({ "DIRECTIONAL_LIGHT", pLight3 });

	std::shared_ptr<LIGHT> pLight4 = std::make_shared<LIGHT>();

	pLight4->m_bEnable         = true;
	pLight4->m_nType           = SPOT_LIGHT;
	pLight4->m_fRange          = 600.0f;
	pLight4->m_xmf4Ambient     = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pLight4->m_xmf4Diffuse     = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	pLight4->m_xmf4Specular    = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	pLight4->m_xmf3Position    = XMFLOAT3(50.0f, 30.0f, 30.0f);
	pLight4->m_xmf3Direction   = XMFLOAT3(0.0f, 1.0f, 1.0f);
	pLight4->m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	pLight4->m_fFalloff        = 8.0f;
	pLight4->m_fPhi            = (float)cos(XMConvertToRadians(90.0f));
	pLight4->m_fTheta          = (float)cos(XMConvertToRadians(30.0f));

	m_pLights.insert({ "SPOT_LIGHT_2", pLight4 });


	CreateShaderVariables();

	return true;

}

void CLight::CreateShaderVariables()
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbLights = ResourceManager::GetInst()->CreateBufferResource(NULL
																	, ncbElementBytes
																	, D3D12_HEAP_TYPE_UPLOAD
																	, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
																	, nullptr);


	m_pd3dcbLights->Map(0, NULL, (void**)&m_pcbMappedLights);

}

void CLight::UpdateShaderVariables()
{
	int nLights  = (int)m_pLights.size();
	int LightIdx = 0;


	for (auto iter = m_pLights.begin(); iter != m_pLights.end(); ++iter) {
		::memcpy(&(m_pcbMappedLights->m_pLights[LightIdx++]), iter->second.get(), sizeof(LIGHT));
	}
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &nLights, sizeof(int));

}

void CLight::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CLight::UpdateGraphicsRootConstantBufferView()
{

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	DX12_COMMAND_LIST->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
}

bool CLight::UpdateLightPosition(std::string Keyname, XMFLOAT3 Pos)
{
	auto iter = m_pLights.find(Keyname);
	if (iter == m_pLights.end())
		return false;

	std::shared_ptr<LIGHT> pLight = iter->second;
	pLight->m_xmf3Position = Pos;
	return true;

}

bool CLight::UpdateLightDirection(std::string Keyname, XMFLOAT3 Dir)
{
	auto iter = m_pLights.find(Keyname);
	if (iter == m_pLights.end())
		return false;

	std::shared_ptr<LIGHT> pLight = iter->second;
	pLight->m_xmf3Direction = Dir;
	return true;

}

std::shared_ptr<LIGHT> CLight::GetLight(std::string Keyname)
{
	auto iter = m_pLights.find(Keyname);
	if (iter == m_pLights.end())
		return nullptr;
	return iter->second;
}

CLight::CLight()
{
}

CLight::~CLight()
{
	for (auto iter = m_pLights.begin(); iter != m_pLights.end(); ++iter)
	{
		iter->second.reset();
	}
}
