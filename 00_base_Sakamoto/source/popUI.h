//============================================
//
//	�����o���^UI [popUI.h]
//	Author:���������Y
//
//============================================
#ifndef _POPUI_H_
#define _POPUI_H_

#include "main.h"
#include "objectBillboard.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CPopUi : public CObjectBillboard
{
public:

	CPopUi(int nPriority = 6);
	~CPopUi();

	static CPopUi* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	float GetStateCount(void) { return m_fStateCountParcent; }
	void SetStateCount(int StateCount) { m_fStateCountParcent = StateCount; }


private:
	float m_fStateCountParcent;		//�\���˕ێ���Ԃ̃J�E���g�̊���

};
#endif