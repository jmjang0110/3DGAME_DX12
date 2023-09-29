#pragma once
/*
		 Descriptor Range 를 통해서 Resource 를 연결한다. 
		 Texture 를 연결할 때는 Root Descriptor Root Constant 를 써도 되는데
		 여러 파라미터들을 쉐이더를 넘길 수 욌도록 세가지 방법을 제공한다. 
		 그러면 
			Descriptor Heap				Resource 
	[]	-------> [SRV] UAV, SRV등		-----> []
	[]			 [CBC]
	[]			 [
	[]
	[]
	[]


	Dsecriptor SRV - Resource 를 Ragne 를 통해서 넘기겠다. 
	Descriptor Heap -----> 시작주소를 나중에 Set 
								[]
					--------->	[CBV]	----------> Range 로 표현 
								[CBV]
								[CBV]
								[]
*/

/// 일반적으로 셰이더 프로그램은 특정 자원들 ( Constant Buffer, Texture, SamplerState 등)이
/// 입력된다고 기대한다. RootSignature는 셰이더 프로그램이 기대하는 자원들을 정의한다.
/// 셰이더 프로그램은 본질적으로 하나의 함수이고 셰이더에 입력되는 자원들은 함수의 매개변수들에 해당하므로,
/// RootSignature는 곧 함수 서명을 정의하는 수단이라 할 수있다.
/// 
class CRootSignature
{
private:

private:
	ComPtr<ID3D12RootSignature>	m_RootSignature{};
	D3D12_ROOT_SIGNATURE_DESC   m_RootSignatureDesc{};

	ComPtr<ID3DBlob>			m_blobSignature{};
	ComPtr<ID3DBlob>			m_blobError{};

public:
	CRootSignature();
	~CRootSignature();


public:
	void OnCreate();

/// [ G E T ]
	ComPtr<ID3D12RootSignature>	GetRootSignature() { return m_RootSignature; }

};

