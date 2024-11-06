//============================================
//
//	���[���u���b�N�̏��� [RailBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "CubeBlock.h"

class CRail;

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CRailBlock : public CCubeBlock
{
public:
	CRailBlock(int nPriority = 3);
	~CRailBlock();

	static CRailBlock* Create(int nMapWight, int nMapHeight, bool Edit, int Max, int *nMove);

	HRESULT Init(int nMapWight, int nMapHeight, bool Edit, int Max = 0, int* nMove = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void EditRailSet(int Number);
	void EditRailUpdate(void);

	// �ÓI�����o�֐�
	static CListManager<CRailBlock>* GetList(void); // ���X�g�擾

private:

	void Move(D3DXVECTOR3 *Pos);
	void RailCheck();
	void RailAddWrite();
	void RailSet(int Max, int* nMove);

	int StartWightNumber;
	int StartHeightNumber;

	bool bMoveOK[4];
	CRail* m_pTop;		// �擪�̃��[���ւ̃|�C���^
	CRail* m_pCur;		// �Ō���̃��[���ւ̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CRailBlock>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRailBlock>::AIterator m_iterator; // �C�e���[�^�[

};
#endif