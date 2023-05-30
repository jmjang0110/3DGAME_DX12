#pragma once
class CDevice
{
private:
	ComPtr<ID3D12Device>	m_pd3dDevice;		//Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	ComPtr<IDXGIFactory4>	m_pdxgiFactory;		//DXGI ���丮 �������̽��� ���� �������̴�. 

public:
	CDevice();
	~CDevice();

public:
	void OnCreate();

public:
	// SET 

	// GET 
	ComPtr<ID3D12Device>	GetDevice();
	ComPtr<IDXGIFactory4>	GetDxgiFactory4();


};

