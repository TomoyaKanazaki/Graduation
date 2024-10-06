//============================================
//
//	���k����r���{�[�h���o���� [shrinkingBillboard.h]
//	Author: Kai Takada
//
//============================================
#ifndef _SHRINKING_BILLBOARD_H_
#define _SHRINKING_BILLBOARD_H_

#include "main.h"
#include "object.h"

//========================
//�N���X��`
//========================
class CObjectBillboard;

//�I�u�W�F�N�g�v���C���[�N���X
class CShrinkingBillboard : public CObject
{
public:

	CShrinkingBillboard(int nPriority = 7);
	~CShrinkingBillboard();

	static CShrinkingBillboard* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRadius(float fRadMin, float fRadMax);
	void SetFrameAll(int nFrame);
	int GetFrame() { return m_nShrinkFrameAll; }
	int GetFrameNow() { return m_nShrinkNow; }
	void SetPos(D3DXVECTOR3 pos);

private:
	int m_nShrinkFrameAll;		//�k�ޑ��t���[����
	int m_nShrinkNow;		//���݂̃t���[��
	float m_fShrinkRadius;		//�t���[���ŏk�܂���T�C�Y
	float m_fHoldTime;

	CObjectBillboard* m_pMax;
	CObjectBillboard *m_pMin;
};
#endif