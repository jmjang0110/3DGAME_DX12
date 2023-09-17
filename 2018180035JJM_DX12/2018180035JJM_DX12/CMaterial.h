#pragma once

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

class CShader;
class CGraphicsPipelineState;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

struct MATERIALLOADINFO
{
	XMFLOAT4						m_xmf4AlbedoColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	float							m_fGlossiness        = 0.0f;
	float							m_fSmoothness        = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic          = 0.0f;
	float							m_fGlossyReflection  = 0.0f;

	UINT							m_nType				= 0x00;

	//char							m_pstrAlbedoMapName[64]       = { '\0' };
	//char							m_pstrSpecularMapName[64]     = { '\0' };
	//char							m_pstrMetallicMapName[64]     = { '\0' };
	//char							m_pstrNormalMapName[64]       = { '\0' };
	//char							m_pstrEmissionMapName[64]     = { '\0' };
	//char							m_pstrDetailAlbedoMapName[64] = { '\0' };
	//char							m_pstrDetailNormalMapName[64] = { '\0' };
};

struct MATERIALSLOADINFO
{
	int								m_nMaterials = 0;
	MATERIALLOADINFO*				m_pMaterials = NULL;
};

struct MATERIAL_COLOR {
	XMFLOAT4						m_xmf4Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4						m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};

class CMaterial
{
public:
	std::string m_Name;;

private:
	std::shared_ptr<CGraphicsPipelineState>  m_PipelineState;
	MATERIAL_COLOR							 m_tColor; // 이제 텍스처로 ...
	// TEXTURE Add


public:
	void UpdateShaderVariable();
	void OnPrepareRender();

public:
	/// [ S E T ]
	void SetPipelineState(std::shared_ptr<CGraphicsPipelineState> pPipelineState) { m_PipelineState = pPipelineState; }
	void SetColor(MATERIAL_COLOR tColor) { m_tColor = tColor; }

	/// [ G E T ]
	std::shared_ptr<CGraphicsPipelineState> GetPipelineState() { return m_PipelineState; }
	MATERIAL_COLOR GetColor() { return m_tColor; }

public:
	CMaterial();
	~CMaterial();


};

