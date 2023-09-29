#pragma once
#include "ImGui/imgui.h"

class CImGui
{
	SINGLETON_PATTERN(CImGui);
public:
	CImGui();
	~CImGui();

private:
	ComPtr<ID3D12DescriptorHeap> m_pd3dSrvDescHeap{};
	bool						 m_show_demo_window;
	bool						 m_show_another_window;
	ImVec4						 m_clear_color;

	
public:
	bool Init(WindowInfo WinInfo);
	void Progress();
	void Render();


public:
	void DestroyImGui();

};

