
#include "stdafx.h"
#include "CKeyManager.h"
#include "CGameFramework.h"


CKeyManager* CKeyManager::m_pInst = nullptr;
int g_iVK[(UINT)KEY::KEY_END] =
{
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',
	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
	VK_NUMPAD9,
	VK_LSHIFT, VK_LMENU, VK_LCONTROL, VK_SPACE, VK_RETURN, VK_BACK, VK_ESCAPE, VK_TAB, VK_DELETE,
	VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
};

CKeyManager::CKeyManager()
{
}

CKeyManager::~CKeyManager()
{
}



void CKeyManager::Init()
{
	m_vecKey.assign((UINT)KEY::KEY_END, tKeyInfo{ KEY_STATE::NONE, false });
	m_arrEnabled.set();
}

void CKeyManager::Update()
{
	const HWND hFocusedHwnd = GetFocus();

	if (hFocusedHwnd)
	{
		for (size_t i = 0; i < std::size(g_iVK); ++i)
		{
			tKeyInfo& keyInfo = m_vecKey[i];

			// 키가 눌려있다
			if (GetAsyncKeyState(g_iVK[i]) & 0x8000)
			{
				// 이전엔 눌린적이 없다.
				if (false == keyInfo.bPrevCheck)
				{
					keyInfo.eState = KEY_STATE::TAP;
				}
				else
				{
					keyInfo.eState = KEY_STATE::PRESSED;
				}
				keyInfo.bPrevCheck = true;
			}
			// 키가 안눌려있다.
			else
			{
				// 이전엔 눌려있었다.
				if (KEY_STATE::TAP == keyInfo.eState || KEY_STATE::PRESSED == keyInfo.eState)
				{
					keyInfo.eState = KEY_STATE::AWAY;
				}
				else if (KEY_STATE::AWAY == keyInfo.eState)
				{
					keyInfo.eState = KEY_STATE::NONE;
				}
				keyInfo.bPrevCheck = false;
			}
		}

		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(CGameFramework::GetInst()->GetWindowInfo().hWnd, &ptMouse);

		m_vMousePrevPos = m_vMousePos;
		m_vMousePos = XMFLOAT2(static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y));
		m_vMouseDir.x = m_vMousePos.x - m_vMousePrevPos.x;
		m_vMouseDir.y = m_vMousePos.y - m_vMousePrevPos.y;

		m_vMouseDir.y *= -1.f;
	}

	// 윈도우가 포커싱 되어있지 않다.
	else
	{
		for (auto& keyInfo : m_vecKey)
		{
			keyInfo.bPrevCheck = false;
			if (KEY_STATE::TAP == keyInfo.eState || KEY_STATE::PRESSED == keyInfo.eState)
			{
				keyInfo.eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == keyInfo.eState)
			{
				keyInfo.eState = KEY_STATE::NONE;
			}
		}
	}
}

void CKeyManager::SetEnableInput(KEY _key, bool _enable)
{

	m_arrEnabled[(UINT)_key] = _enable;
}

void CKeyManager::EnableAll(bool _enable)
{
	if (_enable)
	{
		m_arrEnabled.set();
	}
	else
	{
		m_arrEnabled.reset();
	}
}
