#include "stdafx.h"
#include "ResourceManager.h"


#include "CDevice.h"
#include "CGameFramework.h"
#include "CCommandQueue.h"
#include "CMesh.h"
#include "CVertex.h"
#include "CGameObject.h"
#include "CShader.h"

#include "CRootSignature.h"
#include "CGraphicsPipelineState.h"
#include "CFileManager.h"
#include "CMaterial.h"



ResourceManager* ResourceManager::m_pInst = nullptr;

ResourceManager::ResourceManager()
{

}
ResourceManager::~ResourceManager()
{

}

void ResourceManager::OnDestroy()
{
	if (m_TriangleMesh)
		m_TriangleMesh.reset();


	for (auto iter = m_Material.begin(); iter != m_Material.end(); ++iter)
		iter->second->~CMaterial();
	m_Material.clear();

	for (auto iter = m_Shaders.begin(); iter != m_Shaders.end(); ++iter)
		iter->second->~CShader();
	m_Shaders.clear();

	for (auto iter = m_PipelineStates.begin(); iter != m_PipelineStates.end(); ++iter)
		iter->second->~CGraphicsPipelineState();
	m_PipelineStates.clear();

	for (auto iter = m_Mesh.begin(); iter != m_Mesh.end(); ++iter)
	{
		iter->second.reset();

	}
	m_Mesh.clear();


}



void ResourceManager::OnCreate()
{
	//CreateTriangleMesh();

	CreateDeafult_RS();
	CreateDefault_BL();
	CreateDefault_IPlayout();
	CreateDefault_DS();

	CreateDeafult_Shaders();
	CreateDeafult_PipelineState();

	Create_Shaders_Illuminate();
	Create_InputLayout_Illuminate();
	Create_PipelineState_Illuminate();


	//CreateCubeMesh(12.f, 12.f, 12.f);

}


ComPtr<ID3D12Resource> ResourceManager::CreateBufferResource(void* pData
															, UINT nBytes
															, D3D12_HEAP_TYPE d3dHeapType
															, D3D12_RESOURCE_STATES d3dResourceStates
															, ComPtr<ID3D12Resource>* pd3dUploadBuffer)
{
	ComPtr<ID3D12Resource> pd3dBuffer{};

	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type                 = d3dHeapType;
	d3dHeapPropertiesDesc.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask     = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
	d3dResourceDesc.Alignment          = 0;
	d3dResourceDesc.Width              = nBytes;
	d3dResourceDesc.Height             = 1;
	d3dResourceDesc.DepthOrArraySize   = 1;
	d3dResourceDesc.MipLevels          = 1;
	d3dResourceDesc.Format             = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count   = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags              = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COMMON;
	//	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
	//	else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

	HRESULT hResult = DEVICE(CGameFramework)->GetDevice()->CreateCommittedResource(&d3dHeapPropertiesDesc
		, D3D12_HEAP_FLAG_NONE
		, &d3dResourceDesc
		, d3dResourceInitialStates
		, NULL, __uuidof(ID3D12Resource)
		, (void**)&pd3dBuffer);
	if (pData)
	{
		switch (d3dHeapType)
		{
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			if (pd3dUploadBuffer)
			{
				d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				DEVICE(CGameFramework)->GetDevice()->CreateCommittedResource(&d3dHeapPropertiesDesc
					, D3D12_HEAP_FLAG_NONE
					, &d3dResourceDesc
					, D3D12_RESOURCE_STATE_GENERIC_READ
					, NULL
					, __uuidof(ID3D12Resource)
					, (void**)pd3dUploadBuffer->GetAddressOf());

				D3D12_RANGE d3dReadRange = { 0, 0 };
				UINT8* pBufferDataBegin = NULL;
				(*pd3dUploadBuffer)->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
				memcpy(pBufferDataBegin, pData, nBytes);
				(*pd3dUploadBuffer)->Unmap(0, NULL);

				COMMAND_LIST(CGameFramework)->CopyResource(pd3dBuffer.Get()
					, pd3dUploadBuffer->Get());

				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));

				d3dResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				d3dResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				d3dResourceBarrier.Transition.pResource   = pd3dBuffer.Get();
				d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				d3dResourceBarrier.Transition.StateAfter  = d3dResourceStates;
				d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				COMMAND_LIST(CGameFramework)->ResourceBarrier(1, &d3dResourceBarrier);
			}
			break;
		}
		case D3D12_HEAP_TYPE_UPLOAD:
		{
			D3D12_RANGE d3dReadRange = { 0, 0 };
			UINT8* pBufferDataBegin = NULL;
			pd3dBuffer->Map(0, &d3dReadRange, (void**)&pBufferDataBegin);
			memcpy(pBufferDataBegin, pData, nBytes);
			pd3dBuffer->Unmap(0, NULL);
			break;
		}
		case D3D12_HEAP_TYPE_READBACK:
			break;
		}
	}

	return(pd3dBuffer);
}

void ResourceManager::CreateTriangleMesh()
{
	m_TriangleMesh = std::make_shared<CMesh>();

	std::vector<CVertex> pVertices;
	pVertices.push_back(CVertex(XMFLOAT3(0.0f, 0.5f, 0.0f)));
	pVertices.push_back(CVertex(XMFLOAT3(0.5f, -0.5f, 0.0f)));
	pVertices.push_back(CVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f)));

	m_TriangleMesh->OnCreate((int)pVertices.size()
							, sizeof(CVertex)
							, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
							, pVertices);

}


void ResourceManager::CreateDeafult_RS()
{
	D3D12_RASTERIZER_DESC RasterizerStateDesc{};

	/// 래스터라이저 상태를 설정한다. 
	::ZeroMemory(&RasterizerStateDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerStateDesc.FillMode              = D3D12_FILL_MODE_SOLID;
	RasterizerStateDesc.CullMode              = D3D12_CULL_MODE_BACK;

	RasterizerStateDesc.FrontCounterClockwise = FALSE;
	RasterizerStateDesc.DepthBias             = 0;
	RasterizerStateDesc.DepthBiasClamp        = 0.0f;
	RasterizerStateDesc.SlopeScaledDepthBias  = 0.0f;
	RasterizerStateDesc.DepthClipEnable       = TRUE;
	RasterizerStateDesc.MultisampleEnable     = FALSE;
	RasterizerStateDesc.AntialiasedLineEnable = FALSE;
	RasterizerStateDesc.ForcedSampleCount     = 0;
	RasterizerStateDesc.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	AddRasterizerDesc("Default", RasterizerStateDesc);

}

void ResourceManager::CreateDefault_BL()
{
	D3D12_BLEND_DESC BlendState{};

	/// 블렌드 상태를 설정한다. 
	::ZeroMemory(&BlendState, sizeof(D3D12_BLEND_DESC));

	BlendState.AlphaToCoverageEnable					= FALSE;
	BlendState.IndependentBlendEnable					= FALSE;

	BlendState.RenderTarget[0].BlendEnable				= FALSE;
	BlendState.RenderTarget[0].LogicOpEnable			= FALSE;
	BlendState.RenderTarget[0].SrcBlend					= D3D12_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend				= D3D12_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOp					= D3D12_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha			= D3D12_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha			= D3D12_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha				= D3D12_BLEND_OP_ADD;
	BlendState.RenderTarget[0].LogicOp					= D3D12_LOGIC_OP_NOOP;
	BlendState.RenderTarget[0].RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;

	AddBlendDesc("Default", BlendState);

}
void ResourceManager::CreateDefault_IPlayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점은 위치 벡터(POSITION)와 색상(COLOR)을 가진다. 
	pd3dInputElementDescs[0] = { "POSITION"
								, 0
								, DXGI_FORMAT_R32G32B32_FLOAT
								, 0
								, 0
								, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
								, 0 };

	pd3dInputElementDescs[1] = { "COLOR"
								, 0
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, 0
								, 12
								, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
								, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc{};

	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements        = nInputElementDescs;

	AddInputLayoutDesc("Default", d3dInputLayoutDesc);


}
void ResourceManager::CreateDefault_DS()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc{};

	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));

	d3dDepthStencilDesc.DepthEnable						= TRUE;
	d3dDepthStencilDesc.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc						= D3D12_COMPARISON_FUNC_LESS;
	
	d3dDepthStencilDesc.StencilEnable					= FALSE;
	d3dDepthStencilDesc.StencilReadMask					= 0x00;
	d3dDepthStencilDesc.StencilWriteMask				= 0x00;
	
	d3dDepthStencilDesc.FrontFace.StencilFailOp			= D3D12_STENCIL_OP_KEEP; 
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp			= D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC_NEVER;
	
	d3dDepthStencilDesc.BackFace.StencilFailOp			= D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp		= D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp			= D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC_NEVER;

	AddDepthStencilDesc("Default", d3dDepthStencilDesc);

}

void ResourceManager::CreateDeafult_Shaders()
{
	std::shared_ptr<CShader> VS = std::make_shared<CShader>();
	std::shared_ptr<CShader> PS = std::make_shared<CShader>();

	VS->OnCreate(SHADER_TYPE::VERTEX_SHADER, L"VertexShader.hlsl", "VS_Main", "vs_5_1");
	PS->OnCreate(SHADER_TYPE::PIXEL_SHADER, L"PixelShader.hlsl", "PS_Main", "ps_5_1");

	AddShader("Deafult_VS", VS);
	AddShader("Default_PS", PS);

}

void ResourceManager::CreateDeafult_PipelineState()
{
	std::shared_ptr<CGraphicsPipelineState> pPS = std::make_shared<CGraphicsPipelineState>();

	pPS->SetVertexShader(ResourceManager::GetInst()->GetShader("Deafult_VS"));
	pPS->SetPixelShader(ResourceManager::GetInst()->GetShader("Default_PS"));
	
	pPS->SetRasterizerState(ResourceManager::GetInst()->GetRasterizerDesc("Default"));
	pPS->SetBlendState(ResourceManager::GetInst()->GetBlendDesc("Default"));
	pPS->SetDepthStencilState(ResourceManager::GetInst()->GetDepthStencilState("Default"));
	pPS->SetInputLayout(ResourceManager::GetInst()->GetInputLayoutDesc("Default"));

	HRESULT hResult = pPS->CreateGraphicsPipelineState();
	if (hResult == S_OK) {
		ResourceManager::GetInst()->AddPipelineState("Default", pPS);
	}

}

void ResourceManager::Create_PipelineState_Illuminate()
{
	std::shared_ptr<CGraphicsPipelineState> pPS = std::make_shared<CGraphicsPipelineState>();


	pPS->SetVertexShader(ResourceManager::GetInst()->GetShader("Lighting_VS"));
	pPS->SetPixelShader(ResourceManager::GetInst()->GetShader("Lighting_PS"));

	pPS->SetRasterizerState(ResourceManager::GetInst()->GetRasterizerDesc("Default"));
	pPS->SetBlendState(ResourceManager::GetInst()->GetBlendDesc("Default"));
	pPS->SetDepthStencilState(ResourceManager::GetInst()->GetDepthStencilState("Default"));
	pPS->SetInputLayout(ResourceManager::GetInst()->GetInputLayoutDesc("Illuminate"));


	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = pPS->GetPipelineStateDesc();
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	pPS->SetPipelineStateDesc(desc);

	HRESULT hResult = pPS->CreateGraphicsPipelineState();
	if (hResult == S_OK) {
		ResourceManager::GetInst()->AddPipelineState("Illuminate", pPS);
	}

}

void ResourceManager::CreateCubeMesh(float width, float height, float depth)
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
	pMesh->m_Name = "Cube";

	MeshLoadInfo MeshInfo{};
	int _vertices = 8;
	int _stride = sizeof(XMFLOAT3);

	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;

	
	XMFLOAT4 color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	MeshInfo.m_pxmf4Colors = &color;

	MeshInfo.m_pxmf3Positions = new XMFLOAT3[_vertices];

	MeshInfo.m_pxmf3Positions[0]=((XMFLOAT3(-fx, +fy, -fz	)));
	MeshInfo.m_pxmf3Positions[1]=((XMFLOAT3( +fx, +fy, -fz)));
	MeshInfo.m_pxmf3Positions[2]=((XMFLOAT3( +fx, +fy, +fz)));
	MeshInfo.m_pxmf3Positions[3]=((XMFLOAT3(-fx, +fy, +fz	)));
	MeshInfo.m_pxmf3Positions[4]=((XMFLOAT3(-fx, -fy, -fz	)));
	MeshInfo.m_pxmf3Positions[5]=((XMFLOAT3(+fx, -fy, -fz	)));
	MeshInfo.m_pxmf3Positions[6]=((XMFLOAT3(+fx, -fy, +fz	)));
	MeshInfo.m_pxmf3Positions[7]=((XMFLOAT3(-fx, -fy, +fz	)));
	
	MeshInfo.m_nType |= VERTEXT_POSITION;
	MeshInfo.m_nVertices = _vertices;

	MeshInfo.m_pxmf3Normals = new XMFLOAT3[_vertices];
	MeshInfo.m_nType |= VERTEXT_NORMAL;
	for (int i = 0; i < _vertices; ++i) {
		MeshInfo.m_pxmf3Normals[i] = Vector3::Normalize(MeshInfo.m_pxmf3Positions[i]);
	}
	//pMesh->OnCreate(_vertices, _stride, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vec);

	

	int _indexNum = 36;
	MeshInfo.m_pnIndices = new UINT[_indexNum];
	MeshInfo.m_pnIndices[0] =(3); MeshInfo.m_pnIndices[1] =(1); MeshInfo.m_pnIndices[2] =(0);
	MeshInfo.m_pnIndices[3] =(2); MeshInfo.m_pnIndices[4] =(1); MeshInfo.m_pnIndices[5] =(3);
	MeshInfo.m_pnIndices[6] =(0); MeshInfo.m_pnIndices[7] =(5); MeshInfo.m_pnIndices[8] =(4);
	MeshInfo.m_pnIndices[9] =(1); MeshInfo.m_pnIndices[10]=(5); MeshInfo.m_pnIndices[11]=(0);
	MeshInfo.m_pnIndices[12]=(3); MeshInfo.m_pnIndices[13]=(4); MeshInfo.m_pnIndices[14]=(7);
	MeshInfo.m_pnIndices[15]=(0); MeshInfo.m_pnIndices[16]=(4); MeshInfo.m_pnIndices[17]=(3);
	MeshInfo.m_pnIndices[18]=(1); MeshInfo.m_pnIndices[19]=(6); MeshInfo.m_pnIndices[20]=(5);
	MeshInfo.m_pnIndices[21]=(2); MeshInfo.m_pnIndices[22]=(6); MeshInfo.m_pnIndices[23]=(1);
	MeshInfo.m_pnIndices[24]=(2); MeshInfo.m_pnIndices[25]=(7); MeshInfo.m_pnIndices[26]=(6);
	MeshInfo.m_pnIndices[27]=(3); MeshInfo.m_pnIndices[28]=(7); MeshInfo.m_pnIndices[29]=(2);
	MeshInfo.m_pnIndices[30]=(6); MeshInfo.m_pnIndices[31]=(4); MeshInfo.m_pnIndices[32]=(5);
	MeshInfo.m_pnIndices[33]=(7); MeshInfo.m_pnIndices[34]=(4); MeshInfo.m_pnIndices[35]=(6);


	MeshInfo.m_nIndices = _indexNum;

	pMesh->OnCreate(&MeshInfo);
	pMesh->CreateIndexBufferResource(&MeshInfo);
	//pMesh->CreateNormalBufferResource(&MeshInfo);
	pMesh->CreateNormalBufferResource(&MeshInfo);
	ResourceManager::GetInst()->AddMesh("Cube", pMesh);


}

void ResourceManager::Create_Shaders_Illuminate()
{

	std::shared_ptr<CShader> VS = std::make_shared<CShader>();
	std::shared_ptr<CShader> PS = std::make_shared<CShader>();

	VS->OnCreate(SHADER_TYPE::VERTEX_SHADER, L"VertexShader.hlsl", "VSLighting", "vs_5_1");
	PS->OnCreate(SHADER_TYPE::PIXEL_SHADER, L"PixelShader.hlsl", "PSLighting", "ps_5_1");

	AddShader("Lighting_VS", VS);
	AddShader("Lighting_PS", PS);
}

void ResourceManager::Create_InputLayout_Illuminate()
{

	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION"
								, 0
								, DXGI_FORMAT_R32G32B32_FLOAT
								, 0
								, 0
								, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
								, 0 };

	pd3dInputElementDescs[1] = { "NORMAL"
								, 0
								, DXGI_FORMAT_R32G32B32_FLOAT
								, 1
								, 0
								, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
								, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc{};
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements        = nInputElementDescs;

	AddInputLayoutDesc("Illuminate", d3dInputLayoutDesc);

}

D3D12_RASTERIZER_DESC ResourceManager::GetRasterizerDesc(std::string Name)
{
	D3D12_RASTERIZER_DESC RS{};

	auto iter = m_RSmap.find(Name);
	if (iter != m_RSmap.end())
		RS = iter->second;
	return RS;
}

D3D12_BLEND_DESC ResourceManager::GetBlendDesc(std::string Name)
{
	D3D12_BLEND_DESC BL{};
	auto iter = m_BLmap.find(Name);
	if (iter != m_BLmap.end())
		BL = iter->second;
	return BL;

}

D3D12_INPUT_LAYOUT_DESC ResourceManager::GetInputLayoutDesc(std::string Name)
{
	D3D12_INPUT_LAYOUT_DESC IPlayout{};
	auto iter = m_LOmap.find(Name);
	if (iter != m_LOmap.end())
		IPlayout = iter->second;
	return IPlayout;
}

D3D12_DEPTH_STENCIL_DESC ResourceManager::GetDepthStencilState(std::string Name)
{
	D3D12_DEPTH_STENCIL_DESC DS{};
	auto iter = m_DSmap.find(Name);
	if (iter != m_DSmap.end())
		DS = iter->second;
	return DS;
}

std::shared_ptr<CShader> ResourceManager::GetShader(std::string KeyName)
{
	std::shared_ptr<CShader> pShader = std::make_shared<CShader>();

	auto iter = m_Shaders.find(KeyName);
	if (iter != m_Shaders.end())
	{
		pShader = iter->second;
		return pShader;
	}
	else
	{
		pShader.reset();
		return nullptr;
	}

}

std::shared_ptr<CMesh> ResourceManager::GetMesh(std::string KeyName)
{

	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();
	auto iter = m_Mesh.find(KeyName);
	if (iter != m_Mesh.end())
	{
		pMesh = iter->second;
		return pMesh;

	}
	else
	{
		pMesh.reset();
		return nullptr;
	}


}

std::shared_ptr<CMaterial> ResourceManager::GetMaterial(std::string KeyName)
{

	std::shared_ptr<CMaterial> pMtrl = std::make_shared<CMaterial>();
	auto iter = m_Material.find(KeyName);
	if (iter != m_Material.end())
	{
		pMtrl = iter->second;
		return pMtrl;

	}
	else
	{
		pMtrl.reset();
		return nullptr;
	}


}

std::shared_ptr<CGraphicsPipelineState> ResourceManager::GetPipelineState(std::string KeyName)
{

	std::shared_ptr<CGraphicsPipelineState> pPS = std::make_shared<CGraphicsPipelineState>();
	auto iter = m_PipelineStates.find(KeyName);
	if (iter != m_PipelineStates.end())
	{
		pPS = iter->second;
		return pPS;

	}
	else
	{
		pPS.reset();
		return nullptr;
	}


}



void ResourceManager::AddRasterizerDesc(std::string Keyname, D3D12_RASTERIZER_DESC Desc)
{
	m_RSmap.insert({ Keyname, Desc });

}

void ResourceManager::AddBlendDesc(std::string Keyname, D3D12_BLEND_DESC Desc)
{
	m_BLmap.insert({ Keyname, Desc });

}

void ResourceManager::AddInputLayoutDesc(std::string Keyname, D3D12_INPUT_LAYOUT_DESC Desc)
{
	m_LOmap.insert({ Keyname, Desc });

}

void ResourceManager::AddDepthStencilDesc(std::string Keyname, D3D12_DEPTH_STENCIL_DESC Desc)
{
	m_DSmap.insert({ Keyname, Desc });

}

void ResourceManager::AddShader(std::string Keyname, std::shared_ptr<CShader> pShader)
{
	m_Shaders.insert({ Keyname, pShader });

}

void ResourceManager::AddPipelineState(std::string Keyname, std::shared_ptr<CGraphicsPipelineState> ps)
{
	m_PipelineStates.insert({ Keyname, ps });

}

void ResourceManager::AddMesh(std::string Keyname, std::shared_ptr<CMesh> pMesh)
{
	m_Mesh.insert({ Keyname, pMesh});
}

void ResourceManager::AddMaterial(std::string Keyname, std::shared_ptr<CMaterial> pMtrl)
{
	m_Material.insert({ Keyname, pMtrl });

}
