//============================================
//
//	�����o���^UI�w�i [popUIBg.h]
//	Author:���������Y
//
//============================================
#ifndef _POPUIBG_H_
#define _POPUIBG_H_

#include "main.h"
#include "objectBillboard.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CPopUiBg : public CObjectBillboard
{
public:

	CPopUiBg(int nPriority = 6);
	~CPopUiBg();

	static CPopUiBg* Create(int nPriority = 6);

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