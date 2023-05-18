#pragma once
class CDevice
{
private:
	ID3D12Device*	m_pd3dDevice;		//Direct3D 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다.
	IDXGIFactory4*	m_pdxgiFactory;		//DXGI 팩토리 인터페이스에 대한 포인터이다. 

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

