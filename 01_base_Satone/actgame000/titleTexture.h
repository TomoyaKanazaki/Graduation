//==============================================================
//
//�^�C�g���̃e�N�X�`������[titleTexture.h]
//Author:����������
//
//==============================================================
#ifndef _TITLETEXTURE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TITLETEXTURE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�}�N����`
#define NUM_TITLE_TEX		(2)		//�^�C�g���̃e�N�X�`���̐�

//�O���錾
class CObject2D;		//�I�u�W�F�N�g2D

//�^�C�g���e�N�X�`���N���X
class CTitleTex : public CObject
{
public:
	CTitleTex();	//�R���X�g���N�^
	~CTitleTex();	//�f�X�g���N�^

	static CTitleTex *Create(void);		//��������

	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��

private:
	static CObject2D *m_apObject2D[NUM_TITLE_TEX];		//�I�u�W�F�N�g2D�̃|�C���^
	int m_nIdxTexture[NUM_TITLE_TEX];		//�e�N�X�`���̔ԍ�
	int m_nCntAppear;				//�_�ŃJ�E���^�[
	bool m_bAppear;					//�_�ł̐؂�ւ�
	float m_fAlpha;					//�A���t�@�l
	STATE m_state;					//���
};

#endif
