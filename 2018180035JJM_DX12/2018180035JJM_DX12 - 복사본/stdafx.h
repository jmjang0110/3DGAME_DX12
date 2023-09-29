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
#include <filesystem>

#include <string>
#include <wrl.h>
#include <shellapi.h>

#include <iostream>
#include <vector>
#include <array>
#include <map>

#include <utility>

#include <minwindef.h>

#include <bitset>
#include <random>


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

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>


// IMPORT LIB
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif


//#define RANDOM_COLOR			(0xFF000000 | ((rand() * 0xFFFFFF) / RAND_MAX))
#define DegreeToRadian(x)		float((x)*3.141592654f/180.0f)
#define EPSILON					1.0e-6f


// WINDOW SIZE 
#define FRAME_BUFFER_WIDTH	1200
#define FRAME_BUFFER_HEIGHT 700
#define ASPECT_RATIO (float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

//#define _WITH_CB_GAMEOBJECT_32BIT_CONSTANTS
//#define _WITH_CB_GAMEOBJECT_ROOT_DESCRIPTOR
#define _WITH_CB_WORLD_MATRIX_DESCRIPTOR_TABLE


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
#define COMMAND_LIST(TYPE) TYPE::GetInst()->GetCmdQueue()->GetCommandList()
#define SWAP_CHAIN(TYPE) TYPE::GetInst()->GetSwapChain()
#define ROOT_SIGNATURE(TYPE) TYPE::GetInst()->GetRootSignature()->GetRootSignature()
#define DX12_PIPELINESTATE(TYPE) TYPE::GetInst()->GetGraphicsPipelineStateMachine()

#define DX12_DEVICE CGameFramework::GetInst()->GetDevice()
#define DX12_COMMAND_QUEUE CGameFramework::GetInst()->GetCmdQueue()
#define DX12_COMMAND_LIST CGameFramework::GetInst()->GetCmdQueue()->GetCommandList()
#define DX12_SWAP_CHAIN CGameFramework::GetInst()->GetSwapChain()
#define DX12_ROOT_SIGNATURE CGameFramework::GetInst()->GetRootSignature()->GetRootSignature()

#define RES_MGR ResourceManager::GetInst()
#define FILE_MGR CFileManager::GetInst()



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

#define MAIN_CAMERA(TYPE) TYPE::GetInst()->GetCurScene()->GetMainCamera()
#define GAME_OBJECT(TYPE, NAME) TYPE::GetInst()->GetCurScene()->GetObjByName(NAME)


#define KEY_TAP(key)     KEY_STATE::TAP == CKeyManager::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyManager::GetInst()->GetKeyState(key)
#define KEY_AWAY(key)    KEY_STATE::AWAY == CKeyManager::GetInst()->GetKeyState(key)

#define MAX_LIGHTS			16

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

#define RANDOM_COLOR			XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))


#define NUM_FRAMES_IN_FLIGHT 3

// Texture Option
#define TEXTURES 6

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

// 각종 typedef
using int8   = __int8;
using int16  = __int16;
using int32  = __int32;
using int64  = __int64;
using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using Vec2   = XMFLOAT2;
using Vec3   = XMFLOAT3;
using Vec4   = XMFLOAT4;
using Matrix = XMMATRIX;

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0,
	t1,
	t2,
	t3,
	t4,

	END
};



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
	SWAP_CHAIN_BUFFER_COUNT = 3,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = SRV_REGISTER::END,

	END
};

enum class COMPONENT_TYPE {
	TRANSFORM,
	CAMERA,
	END,

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

#define STL_VECTOR_DELETE(vec) \
	for(int i = 0; i < vec.size(); ++i) \
	{									\
		if (vec[i] != nullptr) {		\
			delete vec[i];				\
			vec[i] = nullptr;			\
		}								\
	}									\
	vec.clear();



#define SAFE_DELETE(p)  \
    if (nullptr != p)   \
        delete p;       \
    p = nullptr;


namespace UTILITY {
	inline float RandF(float fMin, float fMax)
	{
		return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
	}
}




inline int ReadUnityBinaryString(FILE* pFile, char* pstrToken, BYTE* pnStrLength)
{
	UINT nReads = 0;
	nReads = (UINT)::fread(pnStrLength, sizeof(BYTE), 1, pFile);
	nReads = (UINT)::fread(pstrToken, sizeof(char), *pnStrLength, pFile);
	pstrToken[*pnStrLength] = '\0';

	return(nReads);
}

inline XMFLOAT4 XMVECTORF32toXMFLOAT4(XMVECTORF32 xmf32)
{
	XMFLOAT4 xmf4 = XMFLOAT4(xmf32.f[0], xmf32.f[1], xmf32.f[2], xmf32.f[3]);
	return xmf4;
}



inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float* pfS, float* pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

namespace Vector3
{
	inline bool IsZero(XMFLOAT3& xmf3Vector)
	{
		if (::IsZero(xmf3Vector.x) && ::IsZero(xmf3Vector.y) && ::IsZero(xmf3Vector.z))
			return(true);
		return(false);
	}

	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmvVector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmvVector);
		return(xmf3Result);
	}

	inline XMFLOAT3 ScalarProduct(XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar);
		else
			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, float fScalar)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar));
		return(xmf3Result);
	}

	inline XMFLOAT3 Subtract(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline float DotProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result.x);
	}

	inline XMFLOAT3 CrossProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)))));
		else
			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2))));
		return(xmf3Result);
	}

	inline XMFLOAT3 Normalize(XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 m_xmf3Normal;
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		return(m_xmf3Normal);
	}

	inline float Length(XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return(xmf3Result.x);
	}

	inline float Distance(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMVectorSubtract(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
		return(xmf3Result.x);
	}

	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return(XMVectorGetX(XMVectorACos(xmvAngle)));
	}

	inline float Angle(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1))
			, XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)));
		float fAngle = XMVectorGetX(XMVectorACos(xmvAngle));
		return fAngle;
	}

	inline XMFLOAT3 TransformNormal(XMFLOAT3& xmf3Vector, XMMATRIX& xmxm4x4Transform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmxm4x4Transform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformNormal(XMFLOAT3& xmf3Vector, XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMMATRIX& xmxm4x4Transform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmxm4x4Transform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmf3Result);
	}
}




namespace Vector4
{
	inline XMFLOAT4 Multiply(float fScalar, XMFLOAT4& xmf4Vector)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
		return(xmf4Result);
	}

	inline XMFLOAT4 Add(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) + XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}
}

namespace Matrix4x4
{
	inline XMFLOAT4X4 Identity()
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixIdentity());
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Translate(float x, float y, float z)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixTranslation(x, y, z));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMMATRIX& xmmtxMatrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, XMMATRIX& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * xmmtx4x4Matrix2);
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 RotationYawPitchRoll(float fPitch, float fYaw, float fRoll)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 RotationAxis(XMFLOAT3& xmf3Axis, float fAngle)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixRotationAxis(XMLoadFloat3(&xmf3Axis), XMConvertToRadians(fAngle)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Inverse(XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixInverse(NULL, XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Transpose(XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float fFovAngleY, float fAspectRatio, float fNearZ, float fFarZ)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixPerspectiveFovLH(XMConvertToRadians(fFovAngleY), fAspectRatio, fNearZ, fFarZ));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 LookAtLH(XMFLOAT3& xmf3EyePosition, XMFLOAT3& xmf3LookAtPosition, XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 LookToLH(XMFLOAT3& xmf3EyePosition, XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookToLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookTo), XMLoadFloat3(&xmf3UpDirection)));
		return(xmmtx4x4Result);
	}
}

namespace Triangle
{
	inline bool Intersect(XMFLOAT3& xmf3RayPosition, XMFLOAT3& xmf3RayDirection, XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, float& fHitDistance)
	{
		return(TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition), XMLoadFloat3(&xmf3RayDirection), XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fHitDistance));
	}
}

namespace Plane
{
	inline XMFLOAT4 Normalize(XMFLOAT4& xmf4Plane)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
		return(xmf4Result);
	}
}
