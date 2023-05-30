// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <iostream>



// DIRECT X12
#include <d3d12.h> //  Direct3D 12 API 함수를 사용
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DXGIDebug.h>
#include <DirectXColors.h>
#include "d3dx12.h"

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

#define DEVICE(TYPE) TYPE::GetInst()->GetDevice()
#define COMMAND_QUEUE(TYPE) TYPE::GetInst()->GetCmdQueue()
#define SWAP_CHAIN(TYPE) TYPE::GetInst()->GetSwapChain()
#define ROOT_SIGNATURE(TYPE) TYPE::GetInst()->GetRootSignature()
#define DX12_PIPELINESTATE(TYPE) TYPE::GetInst()->GetGraphicsPipelineStateMachine()





#define SAFE_DELETE(p)                     \
    if (nullptr != p)                      \
        delete p;                          \
    p = nullptr;

struct WindowInfo
{
	HINSTANCE					hInstance{};
	HWND						hWnd{};
	int							nWndClientWidth{};
	int							nWndClientHeight{};
};




#define KEY_TAP(key)     KEY_STATE::TAP == CKeyManager::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyManager::GetInst()->GetKeyState(key)
#define KEY_AWAY(key)    KEY_STATE::AWAY == CKeyManager::GetInst()->GetKeyState(key)


enum class KEY_STATE
{
	TAP,     // 막 눌림
	PRESSED, // TAP 이후에도 계속 눌려있음
	AWAY,    // 막 뗀 상황
	NONE,    // 안눌린 상태
};

struct tKeyInfo
{
	KEY_STATE eState;     // 현재 키 상태
	bool      bPrevCheck; // 이전 프레임에서 키가 눌렸는지 체크
};


enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	END
};


enum class KEY
{
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	LEFT, RIGHT, UP, DOWN,
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
	LSHFT, LALT, LCTRL, SPACE, ENTER, BACK, ESC, TAB, DEL,
	LBTN, RBTN, MBTN,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	KEY_END,
};

enum class SHADER_TYPE
{
	VERTEX_SHADER,

	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,

	PIXEL_SHADER,

	END,
};

namespace UTILITY {
	inline float RandF(float fMin, float fMax)
	{
		return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
	}
}



//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE
