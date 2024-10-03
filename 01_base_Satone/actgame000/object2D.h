//==============================================================
//
//�I�u�W�F�N�g2D����[object2D.h]
//Author:����������
//
//==============================================================
#ifndef _OBJECT2D_H_		//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT2D_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�I�u�W�F�N�g2D�N���X
class CObject2D : public CObject
{
public:
	CObject2D();	//�R���X�g���N�^
	~CObject2D();	//�f�X�g���N�^

	static CObject2D *Create(void);		//��������

	virtual HRESULT Init();			//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��

	void BindTexture(int nIdxTexture);		//�O������e�N�X�`���w��

	void SetType(TYPE type);		//�I�u�W�F�N�g�̎�ނ̐ݒ�
	void SetDraw(void);

	void SetPosition(D3DXVECTOR3 pos);								//�ʒu�ݒ�
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				//�ړ��ʐݒ�
	void SetColor(D3DXCOLOR col);									//�F�ݒ�
	void SetSize(float fWidth, float fHeight) { m_fHeight = fHeight; m_fWidth = fWidth; }	//�I�u�W�F�N�g�̃T�C�Y�ݒ�
	void SetAnim(int nPatternAnim);						//�A�j���[�V�����ݒ�
	void SetbgMulti(float fTexU, float fTexV);			//���d�w�i�ݒ�

	D3DXVECTOR3 GetPosition(void) { return m_pos; }	//�ʒu�擾
	D3DXVECTOR3 GetMove(void) { return m_move; }	//�ړ��ʎ擾
	D3DXCOLOR GetColor(void) { return m_col; }		//�F�擾

private:

	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
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
