// header.h: ǥ�� �ý��� ���� ����
// �Ǵ� ������Ʈ Ư�� ���� ������ ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <iostream>



// DIRECT X12
#include <d3d12.h> //  Direct3D 12 API �Լ��� ���
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>#include <DirectXCollision.h>
#include <DXGIDebug.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;



// IMPORT LIB
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")


// WINDOW SIZE 
#define FRAME_BUFFER_WIDTH	800
#define FRAME_BUFFER_HEIGHT 600

#define SINGLETON_PATTERN(TYPE)             \
private:									\
	static TYPE* m_pInst;					\
public:										\
	static TYPE* GetInst()					\
	{										\
		if (!m_pInst) m_pInst = new TYPE;	\
		return m_pInst;						\
	}										\
	static void Destroy() {					\
		if (nullptr != m_pInst) {			\
			delete m_pInst;					\
			m_pInst = nullptr;				\
		}									\
	}						

#define SINGLETON_PATTERN_DEFINITION(TYPE)  \
	static TYPE* TYPE::m_pInst = nullptr;


#define DEVICE(TYPE) TYPE::GetInst()->GetDevice();
#define COMMAND_QUEUE(TYPE) TYPE::GetInst()->GetCmdQueue();
#define SWAP_CHAIN(TYPE) TYPE::GetInst()->GetSwapChain();



#define SAFE_DELETE(p)                     \
    if (nullptr != p)                      \
        delete p;                          \
    p = nullptr;

struct WindowInfo
{
	HINSTANCE					hInstance;
	HWND						hWnd;
	int							nWndClientWidth;
	int							nWndClientHeight;
};

