//==============================================================
//
//���U���g����[result.h]
//Author:����������
//
//==============================================================
#ifndef _RESULT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _RESULT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "manager.h"

//���U���g��ʃN���X�̒�`
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif

