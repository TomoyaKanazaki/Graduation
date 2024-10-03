//==============================================================
//
//�w�i����[bg.h]
//Author:����������
//
//==============================================================
#ifndef _BG_H_		//���̃}�N����`������Ă��Ȃ�������
#define _BG_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//�v���C���[�N���X
class CBg : public CObject2D
{
public:
	CBg();	//�R���X�g���N�^
	~CBg();	//�f�X�g���N�^

	static CBg *Create(void);		//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;
};

#endif
