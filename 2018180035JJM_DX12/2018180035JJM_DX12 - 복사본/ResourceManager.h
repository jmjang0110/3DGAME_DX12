#pragma once

class CMesh;
class CShader;
class CGraphicsPipelineState;
class CMaterial;
class CTexture;


class ResourceManager
{
	SINGLETON_PATTERN(ResourceManager);
public:
	ResourceManager();
	~ResourceManager();



private:
	std::shared_ptr<CMesh> m_TriangleMesh;


	std::map<std::string, std::shared_ptr<CMesh>>					m_Mesh;
	std::map<std::string, std::shared_ptr<CMaterial>>				m_Material;
	std::map<std::string, std::shared_ptr<CTexture>>				m_Texture;

	/// [Name] = Desc   
	std::map<std::string, D3D12_RASTERIZER_DESC>					m_RSmap; /// Rasterizer 
	std::map<std::string, D3D12_BLEND_DESC>							m_BLmap; /// Blend 
	std::map<std::string, D3D12_INPUT_LAYOUT_DESC>					m_LOmap; /// Input Layout
	std::map<std::string, D3D12_DEPTH_STENCIL_DESC>					m_DSmap; /// Depth Stencil State 

	/// [ Name ] = Shader 
	std::map<std::string, std::shared_ptr<CShader>>					m_Shaders; /// Shaders
	std::map<std::string, std::shared_ptr<CGraphicsPipelineState>>	m_PipelineStates; /// Graphics Pipeline State


public:
	void OnCreate();
	void OnDestroy();

	
public:

	ComPtr<ID3D12Resource> CreateBufferResource(	 void*			pData
										,			 UINT			nBytes
										, D3D12_HEAP_TYPE			d3dHeapType			= D3D12_HEAP_TYPE_UPLOAD
										, D3D12_RESOURCE_STATES		d3dResourceStates	= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
										, ComPtr<ID3D12Resource>*	pd3dUploadBuffer	= NULL);
		


// DEFAULT PIPELINE STATE 
	void			CreateDeafult_RS();
	void			CreateDefault_BL();
	void			CreateDefault_IPlayout();
	void			CreateDefault_DS();
	void			CreateDeafult_Shaders();
	void			CreateDeafult_PipelineState();

// ILLUMINATE PIPELINE STATE 
	void			Create_InputLayout_Illuminate();
	void			Create_Shaders_Illuminate();
	void			Create_PipelineState_Illuminate();

// BASIC PIPELINE STATE 
	void			CreateBasicShaders();
	void			CreateBasicPipelineState();
	void			CreateBasicInputLayout();

// CREATE MESH 
	void			CreateCubeMesh(float width = 2.0f, float height = 2.0f, float depth = 2.0f);
	void			CreateSphereMesh();
	void			CreateTriangleMesh();

// LOAD TEXTURES
	void			LoadTextures();


	std::shared_ptr<CMesh> CreateGridTerrainMesh(int KeyNum
		, int xStart
		, int zStart
		, int nWidth
		, int nLength
		, XMFLOAT3 xmf3Scale
		, void* pContext);

	
public:
	/// [ G E T ]

	std::shared_ptr<CMesh>		GetTriangleMesh() { return m_TriangleMesh; }
	D3D12_RASTERIZER_DESC		GetRasterizerDesc(std::string Name);
	D3D12_BLEND_DESC			GetBlendDesc(std::string Name);
	D3D12_INPUT_LAYOUT_DESC		GetInputLayoutDesc(std::string Name);
	D3D12_DEPTH_STENCIL_DESC	GetDepthStencilState(std::string Name);
	std::shared_ptr<CShader>    GetShader(std::string KeyName);
	std::shared_ptr<CMesh>		GetMesh(std::string KeyName);
	std::shared_ptr<CMaterial>		GetMaterial(std::string KeyName);
	std::shared_ptr<CGraphicsPipelineState> GetPipelineState(std::string KeyName);
	std::shared_ptr<CTexture>     GetTexture(std::string Keyname);




	void AddRasterizerDesc(std::string Keyname, D3D12_RASTERIZER_DESC Desc);
	void AddBlendDesc(std::string Keyname, D3D12_BLEND_DESC Desc);
	void AddInputLayoutDesc(std::string Keyname, D3D12_INPUT_LAYOUT_DESC Desc);
	void AddDepthStencilDesc(std::string Keyname, D3D12_DEPTH_STENCIL_DESC Desc);
	void AddShader(std::string Keyname, std::shared_ptr<CShader> pShader);
	void AddPipelineState(std::string Keyname, std::shared_ptr<CGraphicsPipelineState> ps);
	void AddMesh(std::string Keyname, std::shared_ptr<CMesh> pMesh);
	void AddMaterial(std::string Keyname, std::shared_ptr<CMaterial> pMtrl);
	void AddTexture(std::string Keyname, std::shared_ptr<CTexture> pTexture);


};

