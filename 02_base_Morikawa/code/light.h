//=======================================
//
//�V���[�e�B���O�A�N�V����[light.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _LIGHT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _LIGHT_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
// �萔��`
//=======================================
namespace
{
const int MAX_LIGHT = 3;	// ���C�g�̍ő吔
}

//=======================================
//���C�g�N���X
//=======================================
class CLight
{
public:
	CLight();	//�R���X�g���N�^
	~CLight();	//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	//�����o�ϐ�
	D3DLIGHT9 m_alight[MAX_LIGHT];	//���C�g�̏��
};

#endif