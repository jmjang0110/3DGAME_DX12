#pragma once

#include "Utility.h"

class CKeyManager
{
	SINGLETON_PATTERN(CKeyManager);

public:
	CKeyManager();
	~CKeyManager();

private:
	std::vector<tKeyInfo> m_vecKey;
	XMFLOAT2             m_vMousePos;
	XMFLOAT2             m_vMousePrevPos;
	XMFLOAT2             m_vMouseDir;

	std::bitset<(UINT)KEY::KEY_END> m_arrEnabled;

public:
	void Init();
	void Update();

	void SetEnableInput(KEY _key, bool _enable);
	void EnableAll(bool _enable);

public:
	KEY_STATE GetKeyState(KEY _eKey) const
	{
		if (m_arrEnabled[(UINT)_eKey])
		{
			return m_vecKey[(UINT)_eKey].eState;
		}
		return KEY_STATE::NONE;
	}

	XMFLOAT2 GetMousePos() const { return m_vMousePos; }
	XMFLOAT2 GetMouseDir() const { return m_vMouseDir; }

	XMFLOAT2 GetMouseDelta() const
	{
		XMFLOAT2 vDelta = XMFLOAT2(m_vMousePos.x - m_vMousePrevPos.x, m_vMousePos.y - m_vMousePrevPos.y);
		return vDelta;
	}

};

