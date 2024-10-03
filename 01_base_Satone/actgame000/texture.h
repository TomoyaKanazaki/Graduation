//==============================================================
//
//�e�N�X�`���̏���[texture.h]
//Author:����������
//
//==============================================================
#ifndef _TEXTURE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TEXTURE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_TEX		(128)		//�e�N�X�`���̍ő吔

//�e�N�X�`���N���X�̒�`
class CTexture
{
public:
	//�e�N�X�`���̎��
	enum TEX
	{
		TEX_PLAYER = 0,		//�v���C���[
		TEX_ENEMY,			//�G
		TEX_FIELD,			//��
		TEX_WALL,			//��
		TEX_BLOCK,			//�u���b�N
		TEX_BULLET,			//�e
		TEX_WATER,			//��
		TEX_ICE,			//�X
		TEX_VAPOR,			//���C
		TEX_EFFECT,			//�G�t�F�N�g
		TEX_SCORE,			//�X�R�A
		TEX_MAX
	};

	CTexture();		//�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^

	HRESULT Load(void);		//�e�N�X�`���ǂݍ��ݏ���
	void Unload(void);		//�e�N�X�`���j������

	int Regist(const char *pFilename);				//�e�N�X�`���̔ԍ��w��
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);		//�e�N�X�`���̏��擾

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];
	static int m_nNumAll;		//�e�N�X�`���̐�
	static const char *m_apFilename[MAX_TEX];		//�e�N�X�`���̖��O
	int m_texType;				//�e�N�X�`���^�C�v
};

#endif
