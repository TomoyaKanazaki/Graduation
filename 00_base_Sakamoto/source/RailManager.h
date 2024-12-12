//============================================
//
//	���[���}�l�[�W���[ [RailManager.h]
//	Author:Satone Shion
//
//============================================
#ifndef _RAILMANAGER_H_
#define _RAILMANAGER_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	static CRailManager* Create();

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetPrevRailManager(CRailManager *Set) { m_pPrev = Set; }
	CRailManager* GetPrevRailManager(void) { return m_pPrev; }
	void SetNextRailManager(CRailManager* Set) { m_pNext = Set; }
	CRailManager* GetNextRailManager(void) { return m_pNext; }

	int GetNextNumber(void) { return m_bNextNumber; }

	// �ÓI�����o�֐�
	static CListManager<CRailManager>* GetList(void); // ���X�g�擾

private:

	CObjectX* m_pRailManagerModel[2];
	int m_bNextNumber;

	int m_nMapWidth;	// �}�b�v�̉��ԍ�
	int m_nMapHeight;	// �}�b�v�̏c�ԍ�
								   
	CRailManager* m_pPrev;		// �O�̃��[���ւ̃|�C���^
	CRailManager* m_pNext;		// ���̃��[���ւ̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CRailManager>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRailManager>::AIterator m_iterator; // �C�e���[�^�[

};
#endif