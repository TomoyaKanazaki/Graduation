//============================================
//
//	�l���X�R�A��UI [getscoreUi.h]
//	Author:���������Y
//
//============================================
#ifndef _GETSCOREUI_H_
#define _GETSCOREUI_H_

#include "main.h"
#include "object.h"

//�}�N����`
#define MAX_NUM (4)
//�O���錾
class CNumBillboard;

//�I�u�W�F�N�g�v���C���[�N���X
class CGetscoreUi : public CObject
{
public:

	CGetscoreUi(int nPriority = 6);
	~CGetscoreUi();

	static CGetscoreUi* Create(int nGetScore,D3DXVECTOR3 pos,int nPriority = 6);

	HRESULT Init(void) ;
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CNumBillboard* m_pNum[MAX_NUM];
	int m_nLife;		//�I�u�W�F�N�g�̎���

};
#endif