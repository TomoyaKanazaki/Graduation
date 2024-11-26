//============================================
//
//	�ǃ��f���̏��� [wall.h]
//	Author:Satone Shion
//
//============================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//�ǃN���X
class CWall : public CObjectX
{
public:

	CWall(int nPriority = 3);
	CWall(int nPriority, CMapSystem::GRID gridCenter);
	~CWall();

	static CWall* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CMapSystem::GRID GetGrid(void) { return m_Grid; }	// �O���b�h�擾

	// �ÓI�����o�֐�
	static CListManager<CWall>* GetList(void); // ���X�g�擾

private:

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	// �ÓI�����o�ϐ�
	static CListManager<CWall>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CWall>::AIterator m_iterator; // �C�e���[�^�[

};
#endif