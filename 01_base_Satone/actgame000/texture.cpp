//==============================================================
//
//�e�N�X�`���̏���[texture.cpp]
//Author:����������
//
//==============================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ��錾
int CTexture::m_nNumAll = -1;			//�e�N�X�`���ԍ�
const char *CTexture::m_apFilename[MAX_TEX] =
{
	"data\\TEXTURE\\player000.png",		//�v���C���[
	"data\\TEXTURE\\enemy000.png",		//�G
	"data\\TEXTURE\\000.jpg",			//��
	"data\\TEXTURE\\back00.jpg",		//��
	"data\\TEXTURE\\block000.jpg",		//�u���b�N
	"data\\TEXTURE\\bullet000.png",		//�e
	"data\\TEXTURE\\bullet001.png",		//��
	"data\\TEXTURE\\effect002.png",		//�X
	"data\\TEXTURE\\effect003.png",		//���C

	"data\\TEXTURE\\effect000.jpg",		//�G�t�F�N�g
	"data\\TEXTURE\\score002.png",		//�X�R�A
};

//==============================================================
//�R���X�g���N�^
//==============================================================
CTexture::CTexture()
{
	//�e�N�X�`��������
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		m_apTexture[nCntTex] = NULL;
	}
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTexture::~CTexture()
{

}

//==============================================================
//�e�N�X�`���ǂݍ��ݏ���
//==============================================================
HRESULT CTexture::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] == NULL)
		{//�e�N�X�`���ݒ肳��ĂȂ��Ƃ�

			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				m_apFilename[nCntTex],
				&m_apTexture[nCntTex]);

			m_nNumAll++;		//�e�N�X�`���̐����Z
		}
	}

	return S_OK;
}

//==============================================================
//�e�N�X�`���j������
//==============================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==============================================================
//�e�N�X�`���ԍ��w�菈��
//==============================================================
int CTexture::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (m_apFilename[nCntTex] == pFilename)
		{//�t�@�C�����������Ƃ�

			return nCntTex;		//�ԍ�
		}
		else if (m_apFilename[nCntTex] != pFilename && nCntTex > m_nNumAll)
		{//�t�@�C�������Ⴄ

			m_apFilename[nCntTex] = pFilename;

			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				m_apFilename[nCntTex],
				&m_apTexture[nCntTex]);

			m_nNumAll++;

			return m_nNumAll;		//�e�N�X�`���̔ԍ���Ԃ�
		}
	}

	return m_nNumAll;		//�e�N�X�`���̔ԍ���Ԃ�
}

//==============================================================
//�e�N�X�`���̏��擾
//==============================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];		//�w��̃e�N�X�`����Ԃ�
}