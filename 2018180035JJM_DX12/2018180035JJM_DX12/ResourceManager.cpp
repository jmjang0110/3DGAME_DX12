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
#include "CTexture.h"


#include "CHeightmapGrid.h"
#include "CHeightMapImage.h"
#include "CHeightmapTerrain.h"





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
		if (iter->first == "Cube")
			int i = 0;

		iter->second.reset();
	}
	m_Mesh.clear();


}



void ResourceManager::OnCreate()
{
	//CreateTriangleMesh();
	//CreateCubeMesh(); /// ���⼭ Mesh�� ����� �ȵ� 


	CreateDeafult_RS();
	CreateDefault_BL();
	CreateDefault_IPlayout();
	CreateDefault_DS();

	CreateDeafult_Shaders();
	CreateDeafult_PipelineState();

	Create_Shaders_Illuminate();
	Create_InputLayout_Illuminate();
	Create_PipelineState_Illuminate();


	CreateBasicInputLayout();
	CreateBasicShaders();
	CreateBasicPipelineState();


	LoadTextures();

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
	d3dHeapPropertiesDesc.Type = d3dHeapType;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = nBytes;
	d3dResourceDesc.Height = 1;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COMMON;
	//	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
	//	else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

	HRESULT hResult = DX12_DEVICE->GetDevice()->CreateCommittedResource(&d3dHeapPropertiesDesc
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
				DX12_DEVICE->GetDevice()->CreateCommittedResource(&d3dHeapPropertiesDesc
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

				DX12_COMMAND_LIST->CopyResource(pd3dBuffer.Get()
					, pd3dUploadBuffer->Get());

				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));

				d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				d3dResourceBarrier.Transition.pResource = pd3dBuffer.Get();
				d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
				d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				DX12_COMMAND_LIST->ResourceBarrier(1, &d3dResourceBarrier);
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
	pVertices.push_back(CVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT2(0.f, 1.f)));
	pVertices.push_back(CVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT2(0.f, 1.f)));
	pVertices.push_back(CVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f),  XMFLOAT2(0.f, 1.f)));

	m_TriangleMesh->OnCreate((int)pVertices.size()
							, sizeof(CVertex)
							, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
							, pVertices);

}

void ResourceManager::LoadTextures()
{
	std::shared_ptr<CTexture> pTexture = std::make_shared<CTexture>(1, RESOURCE_TEXTURE2D, 0, 1);
	pTexture->CreateSrvDescriptorHeap();
	pTexture->LoadTextureFromDDSFile(L"Image/Stone.dds", RESOURCE_TEXTURE2D, 0);
	pTexture->CreateShaderResourceViews(3);
	AddTexture("Image/Stone.dds", pTexture);



}


void ResourceManager::CreateDeafult_RS()
{
	D3D12_RASTERIZER_DESC RasterizerStateDesc{};

	/// �����Ͷ����� ���¸� �����Ѵ�. 
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

	/// ���� ���¸� �����Ѵ�. 
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

	//������ ��ġ ����(POSITION)�� ����(COLOR)�� ������. 
	pd3dInputElementDescs[0] = { "POSITION"										/// SemanticName
								, 0												/// SemanticIndex		
								, DXGI_FORMAT_R32G32B32_FLOAT					/// Format
								, 0												/// InputSlot
								, 0												/// AlignedByteOffset
								, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA	/// InputSlotClass
								, 0 };											/// InstanceDataStepRate



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

	pPS->SetVertexShader(RES_MGR->GetShader("Deafult_VS"));
	pPS->SetPixelShader(RES_MGR->GetShader("Default_PS"));
	
	pPS->SetRasterizerState(RES_MGR->GetRasterizerDesc("Default"));
	pPS->SetBlendState(RES_MGR->GetBlendDesc("Default"));
	pPS->SetDepthStencilState(RES_MGR->GetDepthStencilState("Default"));
	pPS->SetInputLayout(RES_MGR->GetInputLayoutDesc("Default"));

	HRESULT hResult = pPS->CreateGraphicsPipelineState();
	if (hResult == S_OK) {
		RES_MGR->AddPipelineState("Default", pPS);
	}

}

void ResourceManager::Create_PipelineState_Illuminate()
{
	std::shared_ptr<CGraphicsPipelineState> pPS = std::make_shared<CGraphicsPipelineState>();


	pPS->SetVertexShader(RES_MGR->GetShader("Lighting_VS"));
	pPS->SetPixelShader(RES_MGR->GetShader("Lighting_PS"));

	pPS->SetRasterizerState(RES_MGR->GetRasterizerDesc("Default"));
	pPS->SetBlendState(RES_MGR->GetBlendDesc("Default"));
	pPS->SetDepthStencilState(RES_MGR->GetDepthStencilState("Default"));
	pPS->SetInputLayout(RES_MGR->GetInputLayoutDesc("Illuminate"));


	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = pPS->GetPipelineStateDesc();
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	pPS->SetPipelineStateDesc(desc);

	HRESULT hResult = pPS->CreateGraphicsPipelineState();
	if (hResult == S_OK) {
		RES_MGR->AddPipelineState("Illuminate", pPS);
	}

}

void ResourceManager::CreateCubeMesh(float width, float height, float depth)
{
	/// ResourceManager OnCreate ���� �ϸ� �ȵȴ�. �ֳ��ϸ� Commandlist �� �����ִ�...

	MeshLoadInfo* pMeshInfo =  new MeshLoadInfo;
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	/*
		 [0]________[1]
		  /|	   /|
		 / |	  / |
	 [3]/__|_____/[2]|[5]
		| /	[4]	 |  /
		|/		 | /
	 [7]|________|/[6]
	  
	
	*/

	pMesh->m_Name = "Cube";
	strcpy_s(pMeshInfo->m_pstrMeshName, "Cube");

// <Bounds>:
	pMeshInfo->m_xmf3AABBCenter = { XMFLOAT3(-37.1882629, 11.1295128,0.601368666) };
	pMeshInfo->m_xmf3AABBExtents = { XMFLOAT3(52.5753098, 12.7004261,8.22197151) };
	pMeshInfo->m_nVertices = 8;

	int _vertices = 8;
	int _stride = sizeof(XMFLOAT3);

	float fx = width * 0.5f;
	float fy = height * 0.5f;
	float fz = depth * 0.5f;

// <Positions>:
	pMeshInfo->m_nType |= VERTEXT_POSITION;
	pMeshInfo->m_pxmf3Positions = new XMFLOAT3[_vertices];

	pMeshInfo->m_pxmf3Positions[0]	=   XMFLOAT3(	-fx, +fy, -fz	);
	pMeshInfo->m_pxmf3Positions[1]	=   XMFLOAT3(	+fx, +fy, -fz	);
	pMeshInfo->m_pxmf3Positions[2]	=   XMFLOAT3(	+fx, +fy, +fz	);
	pMeshInfo->m_pxmf3Positions[3]	=   XMFLOAT3(	-fx, +fy, +fz	);
	pMeshInfo->m_pxmf3Positions[4]	=   XMFLOAT3(	-fx, -fy, -fz	);
	pMeshInfo->m_pxmf3Positions[5]	=   XMFLOAT3(	+fx, -fy, -fz	);
	pMeshInfo->m_pxmf3Positions[6]	=   XMFLOAT3(	+fx, -fy, +fz	);
	pMeshInfo->m_pxmf3Positions[7]	=   XMFLOAT3(	-fx, -fy, +fz	);
	


// <Normals>:
	pMeshInfo->m_nType |= VERTEXT_NORMAL;
	pMeshInfo->m_pxmf3Normals = new XMFLOAT3[_vertices];

	XMFLOAT3 Normal = XMFLOAT3(-fx, +fy, -fz);
	pMeshInfo->m_pxmf3Normals[0] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(+fx, +fy, -fz);
	pMeshInfo->m_pxmf3Normals[1] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(+fx, +fy, +fz);
	pMeshInfo->m_pxmf3Normals[2] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(-fx, +fy, +fz);
	pMeshInfo->m_pxmf3Normals[3] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(-fx, -fy, -fz);
	pMeshInfo->m_pxmf3Normals[4] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(+fx, -fy, -fz);
	pMeshInfo->m_pxmf3Normals[5] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(+fx, -fy, +fz);
	pMeshInfo->m_pxmf3Normals[6] = Vector3::Normalize(Normal);
	 Normal = XMFLOAT3(-fx, -fy, +fz);
	pMeshInfo->m_pxmf3Normals[7] = Vector3::Normalize(Normal);

	

// <Indices>:
	int _indexNum = 36;
	pMeshInfo->m_nIndices = _indexNum;

	pMeshInfo->m_pnIndices = new UINT[_indexNum];
	pMeshInfo->m_pnIndices[0] =(3); pMeshInfo->m_pnIndices[1] =(1); pMeshInfo->m_pnIndices[2] =(0);
	pMeshInfo->m_pnIndices[3] =(2); pMeshInfo->m_pnIndices[4] =(1); pMeshInfo->m_pnIndices[5] =(3);
	pMeshInfo->m_pnIndices[6] =(0); pMeshInfo->m_pnIndices[7] =(5); pMeshInfo->m_pnIndices[8] =(4);
	pMeshInfo->m_pnIndices[9] =(1); pMeshInfo->m_pnIndices[10]=(5); pMeshInfo->m_pnIndices[11]=(0);
	pMeshInfo->m_pnIndices[12]=(3); pMeshInfo->m_pnIndices[13]=(4); pMeshInfo->m_pnIndices[14]=(7);
	pMeshInfo->m_pnIndices[15]=(0); pMeshInfo->m_pnIndices[16]=(4); pMeshInfo->m_pnIndices[17]=(3);
	pMeshInfo->m_pnIndices[18]=(1); pMeshInfo->m_pnIndices[19]=(6); pMeshInfo->m_pnIndices[20]=(5);
	pMeshInfo->m_pnIndices[21]=(2); pMeshInfo->m_pnIndices[22]=(6); pMeshInfo->m_pnIndices[23]=(1);
	pMeshInfo->m_pnIndices[24]=(2); pMeshInfo->m_pnIndices[25]=(7); pMeshInfo->m_pnIndices[26]=(6);
	pMeshInfo->m_pnIndices[27]=(3); pMeshInfo->m_pnIndices[28]=(7); pMeshInfo->m_pnIndices[29]=(2);
	pMeshInfo->m_pnIndices[30]=(6); pMeshInfo->m_pnIndices[31]=(4); pMeshInfo->m_pnIndices[32]=(5);
	pMeshInfo->m_pnIndices[33]=(7); pMeshInfo->m_pnIndices[34]=(4); pMeshInfo->m_pnIndices[35]=(6);


	pMeshInfo->m_nSubMeshes = 0;
	pMeshInfo->m_pnSubSetIndices = nullptr;
	pMeshInfo->m_ppnSubSetIndices = nullptr;


	pMesh->OnCreate(pMeshInfo);
	//pMesh->CreateNormalBufferResource(pMeshInfo);
	RES_MGR->AddMesh("Cube", pMesh);
	pMesh->CreateIndexBufferResource(pMeshInfo);
	pMesh->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void ResourceManager::CreateBasicShaders()
{

	std::shared_ptr<CShader> VS = std::make_shared<CShader>();
	std::shared_ptr<CShader> PS = std::make_shared<CShader>();

	VS->OnCreate(SHADER_TYPE::VERTEX_SHADER, L"VertexShader.hlsl", "VS_Basic", "vs_5_1");
	PS->OnCreate(SHADER_TYPE::PIXEL_SHADER, L"PixelShader.hlsl", "PS_Basic", "ps_5_1");

	AddShader("Basic_VS", VS);
	AddShader("Basic_PS", PS);
}

void ResourceManager::CreateBasicPipelineState()
{
	std::shared_ptr<CGraphicsPipelineState> pPS = std::make_shared<CGraphicsPipelineState>();

	pPS->SetVertexShader(RES_MGR->GetShader("Basic_VS"));
	pPS->SetPixelShader(RES_MGR->GetShader("Basic_PS"));

	pPS->SetRasterizerState(RES_MGR->GetRasterizerDesc("Default"));
	pPS->SetBlendState(RES_MGR->GetBlendDesc("Default"));
	pPS->SetDepthStencilState(RES_MGR->GetDepthStencilState("Default"));
	pPS->SetInputLayout(RES_MGR->GetInputLayoutDesc("basic"));

	HRESULT hResult = pPS->CreateGraphicsPipelineState();
	if (hResult == S_OK) {
		RES_MGR->AddPipelineState("Basic", pPS);
	}

}

void ResourceManager::CreateBasicInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

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

	pd3dInputElementDescs[2] = { "TEXCOORD"
							, 0
							, DXGI_FORMAT_R32G32_FLOAT
							, 0
							, 12
							, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
							, 0 };



	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc{};
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements        = nInputElementDescs;

	AddInputLayoutDesc("basic", d3dInputLayoutDesc);

}

void ResourceManager::CreateSphereMesh()
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();


	UINT iStackCount		= 40; // ���� ���� ����
	UINT iSliceCount		= 40; // ���� ���� ����
	UINT _vertices = iStackCount * iSliceCount + 2;

	std::vector<XMFLOAT3>  vecVtx;
	std::vector<XMFLOAT3>  vecNormal;
	std::vector<UINT> vecIdx;

	// ===========
		// Sphere Mesh
		// ===========
	float fRadius = 0.5f;

	// Top
	XMFLOAT3 vPos = XMFLOAT3(0.f, fRadius, 0.f);
	XMFLOAT3 vNormal = vPos;
	vNormal = Vector3::Normalize(vNormal);
	vecNormal.push_back(vNormal);
	vecVtx.push_back(vPos);

	// Body
	 iStackCount = 40; // ���� ���� ����
	iSliceCount = 40; // ���� ���� ����

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / static_cast<float>(iSliceCount);
	float fUVYStep = 1.f / static_cast<float>(iStackCount);

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			vPos = XMFLOAT3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle),
				fRadius * cosf(i * fStackAngle),
				fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			vNormal = vPos;
			vNormal = Vector3::Normalize(vNormal);
			vecNormal.push_back(vNormal);
			vecVtx.push_back(vPos);
		}
	}

	// Bottom
	vPos = XMFLOAT3(0.f, -fRadius, 0.f);
	vNormal = vPos;
	vNormal = Vector3::Normalize(vNormal);

	vecNormal.push_back(vNormal);
	vecVtx.push_back(vPos);

	// �ε���
	// �ϱ���
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ����
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ������
	UINT iBottomIdx = static_cast<UINT>(vecVtx.size()) - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = std::make_shared<CMesh>();
	pMesh->CreateVertexBufferResource(vecVtx.data(), vecVtx.size(), sizeof(XMFLOAT3));
	pMesh->CreateNormalBufferResource(vecNormal.data(), vecNormal.size(), sizeof(XMFLOAT3));
	pMesh->CreateIndexBufferResource(vecIdx.data(), vecIdx.size(), sizeof(UINT));
	pMesh->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	std::string Key = "Sphere";
	RES_MGR->AddMesh(Key, pMesh);



	vecVtx.clear();
	vecNormal.clear();
	vecIdx.clear();
}

std::shared_ptr<CMesh> ResourceManager::CreateGridTerrainMesh(int KeyNum
	, int xStart
	, int zStart
	, int nWidth
	, int nLength
	, XMFLOAT3 xmf3Scale
	, void* pContext) // CHeightMapImage
{
	CHeightmapGrid HeightmapGrid{};

	MeshLoadInfo* pMeshInfo = new MeshLoadInfo;
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	//���ڴ� �ﰢ�� ��Ʈ������ �����Ѵ�. 
	pMeshInfo->m_nVertices = nWidth * nLength;	//������ ����(����)�� ������ (nWidth * nLength)�̴�. 
	XMFLOAT3* pVertices = new XMFLOAT3[pMeshInfo->m_nVertices];

	pMeshInfo->m_nType |= VERTEXT_NORMAL;
	pMeshInfo->m_pxmf3Normals = new XMFLOAT3[pMeshInfo->m_nVertices];

	float fHeight = 0.0f
		, fMinHeight = +FLT_MAX
		, fMaxHeight = -FLT_MAX;
	/*
		xStart�� zStart�� ������ ���� ��ġ(x-��ǥ�� z-��ǥ)�� ��Ÿ����.
		Ŀ�ٶ� ������ ���ڵ��� ������ �迭�� ���� ��
		�䰡 �ֱ� ������ ��ü �������� �� ������ ���� ��ġ�� ��Ÿ���� ������ �ʿ��ϴ�.
	*/
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
		{
			//������ ���̿� ������ ���� �����κ��� ���Ѵ�.
			XMFLOAT3 xmf3Position = XMFLOAT3((x * xmf3Scale.x)
				, HeightmapGrid.OnGetHeight(x, z, pContext),
				(z * xmf3Scale.z));

			XMFLOAT4 color = HeightmapGrid.OnGetColor(x, z, pContext);

			XMFLOAT4 xmf4Color{};
			xmf4Color = Vector4::Add(color, xmf4Color);

			pVertices[i] = XMFLOAT3(xmf3Position);
			XMFLOAT3 ColorNormal = XMFLOAT3(xmf4Color.x, xmf4Color.y, xmf4Color.z);
			ColorNormal = Vector3::Normalize(ColorNormal);

			pMeshInfo->m_pxmf3Normals[i] = XMFLOAT3(ColorNormal.x, ColorNormal.y, ColorNormal.z);

			if (fHeight < fMinHeight) fMinHeight = fHeight;
			if (fHeight > fMaxHeight) fMaxHeight = fHeight;
		}
	}
	pMeshInfo->m_pxmf3Positions = pVertices;
	//���� �׸��� ������ ����(����)�� �����ϴ� ������ �����ش�.

	
	pMeshInfo->m_nIndices = ((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1);
	UINT* pnIndices = new UINT[pMeshInfo->m_nIndices]; 
	
	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			//Ȧ�� ��° ���̹Ƿ�(z = 0, 2, 4, ...) �ε����� ���� ������ ���ʿ��� ������ �����̴�. 
			for (int x = 0; x < nWidth; x++)
			{
				//ù ��° ���� �����ϰ� ���� �ٲ� ������(x == 0) ù ��° �ε����� �߰��Ѵ�. 
				if ((x == 0) && (z > 0)) pnIndices[j++] = (UINT)(x + (z * nWidth));
				//�Ʒ�(x, z), ��(x, z+1)�� ������ �ε����� �߰��Ѵ�
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			//¦�� ��° ���̹Ƿ�(z = 1, 3, 5, ...) �ε����� ���� ������ �����ʿ��� ���� �����̴�. 
			for (int x = nWidth - 1; x >= 0; x--)
			{
				//���� �ٲ� ������(x == (nWidth-1)) ù ��° �ε����� �߰��Ѵ�. 
				if (x == (nWidth - 1)) pnIndices[j++] = (UINT)(x + (z * nWidth));
				//�Ʒ�(x, z), ��(x, z+1)�� ������ �ε����� �߰��Ѵ�. 
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}
	pMeshInfo->m_pnIndices = pnIndices;
	



	pMesh->OnCreate(pMeshInfo);
	pMesh->CreateNormalBufferResource(pMeshInfo);
	pMesh->CreateIndexBufferResource(pMeshInfo);
	pMesh->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	std::string Key = "HeightmapGridMesh_" + std::to_string(KeyNum);
	RES_MGR->AddMesh(Key, pMesh);

	
	SAFE_DELETE(pVertices);
	SAFE_DELETE(pnIndices);
	return pMesh;

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

std::shared_ptr<CTexture> ResourceManager::GetTexture(std::string Keyname)
{
	std::shared_ptr<CTexture> pTex = std::make_shared<CTexture>();
	auto iter = m_Texture.find(Keyname);
	if (iter != m_Texture.end())
	{
		pTex = iter->second;
		return pTex;

	}
	else
	{
		pTex.reset();
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

void ResourceManager::AddTexture(std::string Keyname, std::shared_ptr<CTexture> pTexture)
{
	m_Texture.insert({ Keyname, pTexture });

}
