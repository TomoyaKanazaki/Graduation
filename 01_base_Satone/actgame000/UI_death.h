//=============================================================================
//
//���SUI���� [UI_death.h]
//Author:����������
//
//=============================================================================
#ifndef _UI_DEATH_H_		//���̃}�N����`������Ă��Ȃ�������
#define _UI_DEATH_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�}�N����`
#define NUM_DEATH_DIGIT		(3)			//���S�̌���

class CNumber;		//����
class CObject2D;	//�I�u�W�F�N�g2D

//���SUI�N���X
class CDeathUI : public CObject
{
public:
	CDeathUI();	//�R���X�g���N�^
	~CDeathUI();	//�f�X�g���N�^

	static CDeathUI *Create(void);	//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);		//�O������e�N�X�`���w��

	static void Set(int nNum);					//���S���ݒ�
	int Get(void) { return m_nNum; }			//���S���擾
	static void Add(int nValue);				//���S�������Đݒ�

private:
	//�e�N�X�`���̎��
	enum TEX
	{
		TEX_DEATH = 0,		//���S�e�N�X�`��
		TEX_NUMBER,			//�����e�N�X�`��
		TEX_MAX
	};

	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	int m_nIdxTex[TEX_MAX];		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;			//�X�R�A�̈ʒu
	static int m_nNum;			//�X�R�A�̒l
	static int m_aTexU[NUM_DEATH_DIGIT];

	static CNumber *m_apNumber[NUM_DEATH_DIGIT];
	static CObject2D *m_pObject2D;
};

#endif