//=======================================
//
//�r���{�[�h[billboard.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _BILLBOARD_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _BILLBOARD_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object3D.h"

//=======================================
//�r���{�[�h�N���X
//=======================================
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 5);	//�R���X�g���N�^
	~CBillboard();	//�f�X�g���N�^

	//�����o�֐�
	static CBillboard *Create(D3DXVECTOR3 pos, float fwidth, float fheight);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(int pTexture);

	void SetPosVertex(float size);
	void SetVertexAnim(int Num, int Cnt);

	void SetSizeVertex(float size, float fheight);
	void SetEffectVertex(float fwidth, float fHeight);

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	float GetSize();

	void SetEnableFront(bool bFront) { m_bFront = bFront; }		// ���ʌ�����


	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol();

private:
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		//���_�����i�[
	D3DXCOLOR m_col;        //�F
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fLength;	//�Ίp���̒���
	float m_fAngle;		//�Ίp���̊p�x
	bool m_bFront;		// ���ʌ�����
};

#endif