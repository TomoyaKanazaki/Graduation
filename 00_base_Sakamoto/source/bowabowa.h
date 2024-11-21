//============================================
//
//	�{���{���̏��� [SampleObjX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOWABOWA_H_
#define _BOWABOWA_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�{���{���N���X
class CBowabowa : public CItem
{
public:

	CBowabowa(int nPriority = 2);
	~CBowabowa();

	// �ÓI�����o�֐�
	static CListManager<CBowabowa>* GetList(void); // ���X�g�擾

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

private:

	// �����o�֐�
	void Move(D3DXVECTOR3& pos) override; // �ړ�����

	// �ÓI�����o�ϐ�
	static CListManager<CBowabowa>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBowabowa>::AIterator m_iterator; // �C�e���[�^�[

};
#endif