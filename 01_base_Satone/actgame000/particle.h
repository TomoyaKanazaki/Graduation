//==============================================================
//
//�p�[�e�B�N���̏���[effect.h]
//Author:����������
//
//==============================================================
#ifndef _PARTICLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PARTICLE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�p�[�e�B�N���N���X
class CParticle : public CObject
{
public:

	CParticle();	//�R���X�g���N�^
	~CParticle();	//�f�X�g���N�^

	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void Set(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//�ݒ菈��
	//void GenerateNone(void);		//�����Ȃ�
	void GenerateWater(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//��
	void GenerateIce(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);			//�X
	void GenerateVapor(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//���C
	void GenerateExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//����
	void GenerateLife(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//��
	void GenerateJump(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//�W�����v
	void GenerateMove(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//����

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXCOLOR m_col;		//�F
	float m_fRadius;		//���a�i�傫���j
	float m_fAngle;			//�p�x
	float m_fMove;			//�ړ���
	int m_nLife;			//�p�[�e�B�N����������܂ł̎���
	int m_type;	//�p�[�e�B�N���̎��

};

#endif