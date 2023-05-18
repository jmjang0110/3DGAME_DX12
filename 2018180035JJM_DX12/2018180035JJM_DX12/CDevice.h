#pragma once
class CDevice
{
private:
	ID3D12Device*	m_pd3dDevice;		//Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	IDXGIFactory4*	m_pdxgiFactory;		//DXGI ���丮 �������̽��� ���� �������̴�. 

public:
	CDevice();
	~CDevice();

public:
	void OnCreate();

public:
	// SET 

	// GET 
	ID3D12Device*	GetDevice();
	IDXGIFactory4*	GetDxgiFactory4();


};

