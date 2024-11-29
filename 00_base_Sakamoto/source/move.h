//============================================
//
//	�ړ����� [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

//�ړ��N���X
class CMove
{
public:

	CMove();
	~CMove();

	static CMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CListManager<CMove>* GetList(void); // ���X�g�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CMove>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CMove>::AIterator m_iterator; // �C�e���[�^�[

};
#endif