//============================================
//
//	���[���̏��� [Rail.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _Rail_H_
#define _Rail_H_

#include "main.h"
#include "object.h"
#include "MapSystem.h"

//�O���錾
class CObjectX;		// �I�u�W�F�N�gX

//==========================================
//  �萔��`
//==========================================
namespace Rail
{
	const int MAX_RAIL = 2;		// 1�}�X�������[���̐�
}

//���[���N���X
class CRail : public CObject
{
public:

	CRail(int nPriority = 3);
	~CRail();

	// ���[���ʒu�̎��
	enum POSTYPE
	{
		POSTYPE_NONE = 0,		// �ǂ��ɂ�����
		POSTYPE_UP,				// ��
		POSTYPE_DOWN,			// ��
		POSTYPE_LEFT,			// ��
		POSTYPE_RIGHT,			// �E
		POSTYPE_MAX,
	};

	static CRail* Create(CMapSystem::GRID grid, POSTYPE PosType0, POSTYPE PosType1);

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	// ���[���̏ꏊ
	void PrevSet(POSTYPE Set);		// �O
	void NextSet(POSTYPE Set);		// ��

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetPrevRail(CRail *Set) { m_pPrev = Set; }		// �O�̃��[���ݒ�
	CRail* GetPrevRail(void) { return m_pPrev; }		// �O�̃��[���擾
	void SetNextRail(CRail* Set) { m_pNext = Set; }		// ���̃��[���ݒ�
	CRail* GetNextRail(void) { return m_pNext; }		// ���̃��[���擾

	void SetRailOK(int nCnt, bool Set) { m_bRail[nCnt] = Set; }
	bool GetRailOK(int nCnt) { return m_bRail[nCnt]; }
	int GetNextNumber(void) { return m_bNextNumber; }		// ���̃��f���̔ԍ�

	// �ÓI�����o�֐�
	static CListManager<CRail>* GetList(void); // ���X�g�擾

private:

	CObjectX* m_pRailModel[Rail::MAX_RAIL];		// ���[�����f���̏��
	bool m_bRail[POSTYPE_MAX];					// ���[���̒u����ʒu�̔���
	int m_bNextNumber;				// ���̃��[���ԍ�

	int m_nMapWidth;	// �}�b�v�̉��ԍ�
	int m_nMapHeight;	// �}�b�v�̏c�ԍ�
								   
	CRail* m_pPrev;		// �O�̃��[���ւ̃|�C���^
	CRail* m_pNext;		// ���̃��[���ւ̃|�C���^

	CMapSystem::GRID m_Grid;			// �O���b�h���
	POSTYPE m_PosType[Rail::MAX_RAIL];	// ���[���ʒu�̎��

	// �ÓI�����o�ϐ�
	static CListManager<CRail>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRail>::AIterator m_iterator; // �C�e���[�^�[

};
#endif