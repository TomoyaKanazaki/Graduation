//============================================
//
//	���[���u���b�N�̏��� [RailBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "ObjectX.h"
#include "MapSystem.h"

class CRail;

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CRailBlock : public CObjectX
{
public:
	CRailBlock(int nPriority = 3);
	CRailBlock(int nPriority, CMapSystem::GRID gridCenter);
	~CRailBlock();

	static CRailBlock* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CMapSystem::GRID GetGrid(void) { return m_Grid; }	// �O���b�h�擾

	/*static CRailBlock* Create(int nMapWight, int nMapHeight, bool Edit, int Max, int *nMove);

	HRESULT Init(int nMapWight, int nMapHeight, bool Edit, int Max = 0, int* nMove = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);*/

	void EditRailSet(int Number);
	void EditRailUpdate(void);
	void RailDelete();
	int GetRailMax() { return m_nMax; }
	int GetRailMove(int nCnt);

	// �ÓI�����o�֐�
	static CListManager<CRailBlock>* GetList(void); // ���X�g�擾

private:

	void Move(D3DXVECTOR3 *Pos);
	void RailCheck();
	void RailAddWrite();
	void RailSet(int Max, int* nMove);

	//CMapSystem::GRID m_StartGrid;	//�J�n���̃O���b�g�ԍ�

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	bool bMoveOK[4];
	int m_nMax;
	int m_nMove[64];
	CRail* m_pTop;		// �擪�̃��[���ւ̃|�C���^
	CRail* m_pCur;		// �Ō���̃��[���ւ̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CRailBlock>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRailBlock>::AIterator m_iterator; // �C�e���[�^�[

};
#endif