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

	CBowabowa(int nPriority = 3);
	~CBowabowa();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	// �ÓI�����o�֐�
	static CListManager<CBowabowa>* GetList(void); // ���X�g�擾

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void Take(void);

	bool Hit(CPlayer* pPlayer) override;

private:
	void StateManager(void);		//��ԊǗ�

	// �ÓI�����o�ϐ�
	static CListManager<CBowabowa>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBowabowa>::AIterator m_iterator; // �C�e���[�^�[

};
#endif