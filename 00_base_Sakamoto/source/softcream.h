//============================================
//
//	�\�t�g�N���[���̏��� [softcream.h]
//	Author:morikawa shunya
//
//============================================
#pragma once
#include "item.h"

//==========================================
//  �N���X��`
//==========================================
class CSoftCream : public CItem
{
public:

	CSoftCream(int nPriority = 3);
	~CSoftCream();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Move();

	bool Hit(CPlayer* pPlayer) override;

	// �ÓI�����o�֐�
	static CListManager<CSoftCream>* GetList(void); // ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CSoftCream>::AIterator m_iterator; // �C�e���[�^�[
	int m_nAppea;		// �o����
	float m_fMove;		// �ړ���
	float m_fConuter;	// �폜�J�E���^�[

	// �ÓI�����o�ϐ�
	static CListManager<CSoftCream>* m_pList; // �I�u�W�F�N�g���X�g

};
