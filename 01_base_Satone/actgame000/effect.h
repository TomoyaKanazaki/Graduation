//==============================================================
//
//�G�t�F�N�g�̏���[effect.h]
//Author:����������
//
//==============================================================
#ifndef _EFFECT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EFFECT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "billboard.h"

//�v���C���[�N���X
class CEffect : public CBillboard
{
public:
	CEffect();	//�R���X�g���N�^
	~CEffect();	//�f�X�g���N�^

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type);		//��������

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXCOLOR m_col;		//�F
	float m_fRadius;		//���a�i�傫���j
	int m_nLife;			//�G�t�F�N�g�̎���
	int m_type;
};

#endif