//============================================
//
//	���[���̏��� [Rail.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _Rail_H_
#define _Rail_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CRail : public CObject
{
public:

	CRail(int nPriority = 3);
	~CRail();

	enum RAIL_POS
	{
		RAIL_POS_UP = 0,
		RAIL_POS_DOWN,
		RAIL_POS_LEFT,
		RAIL_POS_RIGHT,
		RAIL_POS_MAX,
	};

	static CRail* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void PrevSet(RAIL_POS Set);
	void NextSet(RAIL_POS Set);

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetPrevRail(CRail *Set) { m_pPrev = Set; }
	CRail* GetPrevRail(void) { return m_pPrev; }
	void SetNextRail(CRail* Set) { m_pNext = Set; }
	CRail* GetNextRail(void) { return m_pNext; }

	void SetRailOK(int nCnt, bool Set) { m_bRail[nCnt] = Set; }
	bool GetRailOK(int nCnt) { return m_bRail[nCnt]; }
	int GetNextNumber(void) { return m_bNextNumber; }

	// �ÓI�����o�֐�
	static CListManager<CRail>* GetList(void); // ���X�g�擾

private:

	CObjectX* m_pRailModel[2];
	bool m_bRail[RAIL_POS_MAX];
	int m_bNextNumber;

	int m_nMapWidth;	// �}�b�v�̉��ԍ�
	int m_nMapHeight;	// �}�b�v�̏c�ԍ�
								   
	CRail* m_pPrev;		// �O�̃��[���ւ̃|�C���^
	CRail* m_pNext;		// ���̃��[���ւ̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CRail>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRail>::AIterator m_iterator; // �C�e���[�^�[

};
#endif