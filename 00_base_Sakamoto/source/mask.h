//=======================================
//
//�X�e���V���o�b�t�@�ɕK�v��2D�}�X�N[mask.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _MASK_H_	 //���̃}�N����`����`����Ă��Ȃ�������
#define _MASK_H_	 //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "object2d.h"

//=======================================
// �}�X�N�N���X
//=======================================
class CMask : public CObject2D
{
public:
	CMask(int nPriority = 6);	//�R���X�g���N�^
	~CMask();	//�f�X�g���N�^

	static CMask* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//�����o�ϐ�
};

#endif
