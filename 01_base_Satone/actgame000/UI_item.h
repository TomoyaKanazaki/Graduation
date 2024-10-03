//=============================================================================
//
//�A�C�e��UI���� [UI_item.h]
//Author:����������
//
//=============================================================================
#ifndef _UI_ITEM_H_		//���̃}�N����`������Ă��Ȃ�������
#define _UI_ITEM_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�}�N����`
#define NUM_ITEM_DIGIT		(3)			//�A�C�e���̌���

class CNumber;		//����
class CObject2D;	//�I�u�W�F�N�g2D

//�A�C�e��UI�N���X
class CItemUI : public CObject
{
public:
	CItemUI();	//�R���X�g���N�^
	~CItemUI();	//�f�X�g���N�^

	static CItemUI *Create(void);	//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);		//�O������e�N�X�`���w��

	static void Set(int nNum);					//�A�C�e�����ݒ�
	int Get(void) { return m_nNum; }			//�A�C�e�����擾
	static void Add(int nValue);				//�A�C�e���������Đݒ�

private:
	//�e�N�X�`���̎��
	enum TEX
	{
		TEX_ITEM = 0,		//�A�C�e���e�N�X�`��
		TEX_NUMBER,			//�����e�N�X�`��
		TEX_MAX
	};

	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	int m_nIdxTex[TEX_MAX];		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;			//�X�R�A�̈ʒu
	static int m_nNum;			//�X�R�A�̒l
	static int m_aTexU[NUM_ITEM_DIGIT];

	static CNumber *m_apNumber[NUM_ITEM_DIGIT];
	static CObject2D *m_pObject2D;
};

#endif