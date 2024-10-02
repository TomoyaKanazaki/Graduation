//============================================
//
//	�ړ������̃w���vUI [MoveHelpUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MOVEHELPUI_H_
#define _MOVEHELPUI_H_

#include "main.h"
#include "object3D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CMoveHelpUI : public CObject3D
{
public:

	CMoveHelpUI(int nPriority = 4);
	~CMoveHelpUI();

	static CMoveHelpUI* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVerTex(void);

	//�擾�����E�ݒ菈��
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//�T���v���ϐ�
};
#endif