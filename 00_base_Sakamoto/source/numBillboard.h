//============================================
//
//	�����i�r���{�[�h�j�̏��� [numBillboard.h]
//	Author:���������Y
//
//============================================
#ifndef _NUMBILLBOARD_H_
#define _NUMBILLBOARD_H_

#include "main.h"
#include "objectBillboard.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CNumBillboard : public CObjectBillboard
{
public:

	CNumBillboard(int nPriority = 6);
	~CNumBillboard();

	static CNumBillboard *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetNumber(void) { return m_nNumber; }
   	void SetNumber(int number) { m_nNumber = number; }

private:
	int m_nNumber;							//�\�����鐔��
};
#endif