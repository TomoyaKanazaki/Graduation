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
#include "MapSystem.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�����N���X
class CBible : public CItem
{
public:

	CBible(int nPriority = 2);
	~CBible();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

	// �ÓI�����o�֐�
	static CListManager<CBible>* GetList(void); // ���X�g�擾

private:

	// �����o�֐�
	void Move(D3DXVECTOR3& pos) override;
	void Rotation();
	void SetEffect() override;

	// �����o�ϐ�
	CListManager<CBible>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CBible>* m_pList; // �I�u�W�F�N�g���X�g

};

#endif
