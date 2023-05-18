#pragma once
class CShader
{

private:
	ID3D12PipelineState* m_pd3dPipelineState; //그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터이다. 
	
public:

	CShader();
	~CShader();


};

