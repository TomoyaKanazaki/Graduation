//============================================
//
//	�X�N���[���A���[ [scrollarrow.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SCROLLARROW_H_
#define _SCROLLARROW_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CScrollArrow : public CObject2D
{
public:

	CScrollArrow(int nPriority = 6);
	~CScrollArrow();

	//�v���C���[�̏��
	enum STATE
	{
		STATE_UP = 0,	//��
		STATE_DOWN,		//��
		STATE_LEFT,		//��
		STATE_RIGHT,	//��
		STATE_MAX,		//�ő�
	};

	static CScrollArrow* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//�T���v���ϐ�
	STATE m_State;				//���
};
#endif