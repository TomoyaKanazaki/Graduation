//==============================================================
//
//�I�u�W�F�N�g3D����[object3D.h]
//Author:����������
//
//==============================================================
#ifndef _OBJECT3D_H_		//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT3D_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�I�u�W�F�N�g3D�N���X
class CObject3D : public CObject
{
public:
	CObject3D();	//�R���X�g���N�^
	~CObject3D();	//�f�X�g���N�^

	static CObject3D *Create(void);		//��������

	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��

	void BindTexture(int nIdxTexture);					//�O������e�N�X�`���w��

	void SetType(TYPE type) { m_type = type; CObject::SetType(m_type); }		//�I�u�W�F�N�g�̎�ނ̐ݒ�

	void SetSize(float fWidth, float fHeight);			//�I�u�W�F�N�g�̃T�C�Y�ݒ�
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }			//�ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }			//�ʒu�ݒ�
	void SetColor(D3DXCOLOR col);			//�I�u�W�F�N�g�̐F�ݒ�

	D3DXVECTOR3 GetPosition(void) { return m_pos; }				//�ʒu�擾

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXCOLOR m_col;		//�F
	float m_fHeight;		//����
	float m_fWidth;			//����
	TYPE m_type;			//���
};

#endif
