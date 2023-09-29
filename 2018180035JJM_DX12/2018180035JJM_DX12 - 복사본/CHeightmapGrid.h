#pragma once

/*

	������ ������(����: x-����, ����: z-����, ����: y-����) �����̴�. 
	���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������ x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ���´�. 
	��, ���� ������ x-�� ������ ������ 1�� �ƴ϶� ������ ������ x-��ǥ�� �ȴ�. 
	�̷��� �ϸ� ���� ����(���� ����)�� ����ϴ��� ū ũ���� ����(����)�� ������ �� �ִ�.

*/

class CMesh;

class CHeightmapGrid
{
private:
	CMesh* m_GridMesh;

protected:
	//������ ũ��(����: x-����, ����: z-����)�̴�. 
	int m_nWidth;
	int m_nLength;

	XMFLOAT3 m_xmf3Scale;
public:
	CHeightmapGrid();
	virtual ~CHeightmapGrid();

public:
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }
	//������ ��ǥ�� (x, z)�� �� ����(����)�� ���̸� ��ȯ�ϴ� �Լ��̴�. 
	virtual float OnGetHeight(int x, int z, void *pContext);
	//������ ��ǥ�� (x, z)�� �� ����(����)�� ������ ��ȯ�ϴ� �Լ��̴�. 
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);

};
