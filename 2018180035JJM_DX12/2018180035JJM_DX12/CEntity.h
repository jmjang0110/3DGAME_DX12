#pragma once
class CEntity
{
private:
	static UINT		g_iNextID;

private:
	const UINT		m_iID;
	std::string     m_strName;


public:
	void SetName(const std::string& _name) { m_strName = _name; }
	const std::string& GetName() const { return m_strName; }
	UINT GetID() const { return m_iID; }

public:
	virtual void     UpdateData() {}
	//virtual CEntity* Clone() = 0;

public:
	CEntity();
	//CEntity(const CEntity& _origin);
	virtual ~CEntity();

};

