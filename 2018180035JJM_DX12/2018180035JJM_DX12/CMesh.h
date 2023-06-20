#pragma once


#define VERTEXT_POSITION			0x01
#define VERTEXT_COLOR				0x02
#define VERTEXT_NORMAL				0x04

struct MeshLoadInfo
{
	char							m_pstrMeshName[256] = { 0 };

	UINT							m_nType				= 0x00;

	XMFLOAT3						m_xmf3AABBCenter	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents	= XMFLOAT3(0.0f, 0.0f, 0.0f);

	int								m_nVertices			= 0;
	XMFLOAT3*						m_pxmf3Positions	= NULL;
	XMFLOAT4*						m_pxmf4Colors		= NULL;
	XMFLOAT3*						m_pxmf3Normals		= NULL;

	int								m_nIndices			= 0;
	UINT*							m_pnIndices			= NULL;

	int								m_nSubMeshes		= 0;
	int*							m_pnSubSetIndices	= NULL;
	UINT**							m_ppnSubSetIndices	= NULL;
};


class CMesh 
{
public:
	std::string m_Name;


private:
	D3D12_PRIMITIVE_TOPOLOGY	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT m_nSlot		= 0;
	UINT m_nVertices	= 0;
	UINT m_nStride		= 0;
	UINT m_nOffset		= 0;
	UINT m_nType		= 0;

private:
	ComPtr<ID3D12Resource>					m_pd3dVertexBuffer{};
	ComPtr<ID3D12Resource>					m_pd3dVertexUploadBuffer{};
	D3D12_VERTEX_BUFFER_VIEW				m_d3dVertexBufferView{};

	ComPtr<ID3D12Resource>					m_pd3dIndexBuffer{};
	ComPtr<ID3D12Resource>					m_pd3dIndexUploadBuffer{};
	D3D12_INDEX_BUFFER_VIEW					m_d3dIndexBufferView{};


	std::vector<ComPtr<ID3D12Resource>>		m_pd3dSubSet_IndexBuffers{};
	std::vector<ComPtr<ID3D12Resource>>		m_pd3dSubSet_IndexUploadBuffers{};
	std::vector<D3D12_INDEX_BUFFER_VIEW>	m_pd3dSubSet_IndexBufferViews{};

	ComPtr<ID3D12Resource>					m_pd3dNormalBuffer{};
	ComPtr<ID3D12Resource>					m_pd3dNormalUploadBuffer{};
	D3D12_VERTEX_BUFFER_VIEW				m_d3dNormalBufferView{};


	MeshLoadInfo							m_tMeshInfo;
	int										m_nSubMeshes = 0;
	std::vector<int>						m_pnSubSetIndices{};
	int										m_nIndices = 0;

private:
	/// 정점 정보
	std::vector<class CVertex>				m_Vertices;

public:
	CMesh();
	~CMesh();

public:
	void OnCreate(int verticesNum
				, int nStride
				, D3D12_PRIMITIVE_TOPOLOGY topology
				, std::vector<CVertex> vertices);


	void OnCreate(MeshLoadInfo* tMeshInfo);
	void Render(int nSubSet);


public:
	void CreateNormalBufferResource(MeshLoadInfo* pMeshInfo);
	void CreateIndexBufferResource(MeshLoadInfo* pMeshInfo);


public:
	/// [ G E T ]
	UINT GetType() { return(m_nType); }

public:
	void ReleaseUploadBuffers();


public:
	void SetVertices(int _vertices) { m_nVertices = _vertices; }
	void SetStride(int _size) { m_nStride = _size; }
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY _eTopology) { m_d3dPrimitiveTopology = _eTopology; }

	void SetMeshInfo(MeshLoadInfo tMeshinfo) { m_tMeshInfo = tMeshinfo; }
	MeshLoadInfo GetMeshInfo() { return m_tMeshInfo; }


};

