#pragma once

class CMesh
{
private:
	ComPtr<ID3D12Resource> m_pd3dVertexBuffer{};
	ComPtr<ID3D12Resource> m_pd3dVertexUploadBuffer{};
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};
	
	UINT m_nSlot     = 0;
	UINT m_nVertices = 0;
	UINT m_nStride   = 0;
	UINT m_nOffset   = 0;

public:
	CMesh();
	~CMesh();

public:
	void OnCreate();
	void Render();
};

