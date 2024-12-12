//============================================
//
//	�\���˃A�C�e���̏��� [cross.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CROSS_H_
#define _CROSS_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�\���˃N���X
class CCross : public CItem
{
public:

	// �����o�֐�
	CCross(int nPriority = 2);
	~CCross();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

	// �ÓI�����o�֐�
	static CListManager<CCross>* GetList(void); // ���X�g�擾

	void SetEventPos(D3DXVECTOR3 pos) { m_EventPos = pos; }
	D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }

private:

	// �����o�֐�
	void Rotation();
	void SetEffect() override;

	D3DXVECTOR3 m_EventPos;

	// �ÓI�����o�ϐ�
	static CListManager<CCross>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CCross>::AIterator m_iterator; // �C�e���[�^�[

};
#endif