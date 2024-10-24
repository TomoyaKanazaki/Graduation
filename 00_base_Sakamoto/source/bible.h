//============================================
//
//	�����̏��� [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�����N���X
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;

	// �ÓI�����o�֐�
	static CListManager<CBible>* GetList(void); // ���X�g�擾

private:
	void StateManager(void);		//��ԊǗ�

		// �ÓI�����o�ϐ�
	static CListManager<CBible>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBible>::AIterator m_iterator; // �C�e���[�^�[

};

#endif
