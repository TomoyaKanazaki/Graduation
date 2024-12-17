//========================================
//
// �e�N�X�`���Ǘ�[texture.h]
// Author�F�X��x��
//
//========================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//=======================================
// �萔��`
//=======================================
namespace
{
	const int MAX_TEXTURE = 128;	// �e�N�X�`���̍ő吔
}

//========================================
//�e�N�X�`���N���X
//========================================
class CTexture
{
public:
	enum Texture
	{// �e�N�X�`���Ǘ�
		TYPE_TITLE = 0,
		TYPE_MAX
	};

	CTexture();		//�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^

	//�����o�֐�
	HRESULT Load(void);		//�e�N�X�`���ǂݍ���
	void Unload(void);		//�e�N�X�`���j��

	int Regist(std::string pfile);			//�e�N�X�`�����蓖��
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);	//�e�N�X�`���̃A�h���X�擾

private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	//�e�N�X�`���̔z��
	std::string m_apPath[MAX_TEXTURE];
	static int m_nNumAll;		//����
};

#endif
