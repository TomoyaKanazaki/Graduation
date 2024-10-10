//============================================
//
//	�I�u�W�F�N�g2D�̃T���v�� [SampleObj2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _LIFEUI_H_
#define _LIFEUI_H_

#include "main.h"
#include "object2D.h"

class CNumber;

//�I�u�W�F�N�g�v���C���[�N���X
class CLifeUi : public CObject2D
{
public:

	CLifeUi(int nPriority = 6);
	~CLifeUi();

	static CLifeUi* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

	CNumber *GetNumber(void) { return m_pNumber; }

private:
	int m_nSample;							//�T���v���ϐ�
	CNumber* m_pNumber;
};
#endif