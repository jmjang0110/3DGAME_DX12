#pragma once
/*
		 Descriptor Range �� ���ؼ� Resource �� �����Ѵ�. 
		 Texture �� ������ ���� Root Descriptor Root Constant �� �ᵵ �Ǵµ�
		 ���� �Ķ���͵��� ���̴��� �ѱ� �� �뵵�� ������ ����� �����Ѵ�. 
		 �׷��� 
			Descriptor Heap				Resource 
	[]	-------> [SRV] UAV, SRV��		-----> []
	[]			 [CBC]
	[]			 [
	[]
	[]
	[]


	Dsecriptor SRV - Resource �� Ragne �� ���ؼ� �ѱ�ڴ�. 
	Descriptor Heap -----> �����ּҸ� ���߿� Set 
								[]
					--------->	[CBV]	----------> Range �� ǥ�� 
								[CBV]
								[CBV]
								[]
*/

/// �Ϲ������� ���̴� ���α׷��� Ư�� �ڿ��� ( Constant Buffer, Texture, SamplerState ��)��
/// �Էµȴٰ� ����Ѵ�. RootSignature�� ���̴� ���α׷��� ����ϴ� �ڿ����� �����Ѵ�.
/// ���̴� ���α׷��� ���������� �ϳ��� �Լ��̰� ���̴��� �ԷµǴ� �ڿ����� �Լ��� �Ű������鿡 �ش��ϹǷ�,
/// RootSignature�� �� �Լ� ������ �����ϴ� �����̶� �� ���ִ�.
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

