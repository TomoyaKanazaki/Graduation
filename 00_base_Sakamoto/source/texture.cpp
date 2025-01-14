//========================================
//
// �e�N�X�`���Ǘ�[texture.h]
// Author�F�X��x��
//
//========================================

#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
//�萔��`
//========================================
namespace
{
	// �e�N�X�`���ꊇ�Ǘ�
	const std::string str[]
	{
		{ "data\\TEXTURE\\UI\\tutorial_text_00.png" },	// �ړ��̃e�L�X�g
		{ "data\\TEXTURE\\UI\\tutorial_text_01.png" },	// �\���ˍ��W
		{ "data\\TEXTURE\\UI\\tutorial_text_03.png" },	// �{���{���̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_02.png" },	// �U���̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_004.png" },	// �����̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_05.png" },	// �f�r���z�[���̍��W
	};
}

//========================================
//�ÓI�����o�ϐ�
//========================================
int CTexture::m_nNumAll = 0;

//========================================
//�R���X�g���N�^
//========================================
CTexture::CTexture()
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = {};	//�e�N�X�`���̃|�C���^
		m_apPath[nCntTex] = "\0";
	}
}

//========================================
//�f�X�g���N�^
//========================================
CTexture::~CTexture()
{

}

//========================================
//�e�N�X�`���ꊇ�ǂݍ���
//========================================
HRESULT CTexture::Load(void)
{
	////�f�o�C�X�̎擾
	//CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�w��̃e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\NULL.png", &m_apTexture[0]);

	//�������J�E���g�A�b�v
	//m_nNumAll++;

	return S_OK;
}

//========================================
//�e�N�X�`���j��
//========================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumAll; nCntTex++)
	{//�e�N�X�`���̏I������
		if (m_apTexture[nCntTex] != nullptr)
		{//m_apTexture[nCntTex]��nullptr����Ȃ��Ƃ�
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = nullptr;
		}
	}
}

//========================================
//�e�N�X�`���ʊ��蓖��
//========================================
int CTexture::Regist(const std::string pfile)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (m_apPath[nCntTex] == pfile)
		{//�t�@�C��������v�����Ƃ�
			//nCntTex�Ԗڂ�Ԃ�
			return nCntTex;
		}

		if (m_apTexture[nCntTex] == nullptr)
		{//nullptr�̎�
			//CRenderer�^�̃|�C���^
			CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

			//�f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

			//�w��̃e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pfile.c_str(), &m_apTexture[nCntTex]);

			//�t�@�C����������
			m_apPath[nCntTex] = pfile;

			//�������J�E���g�A�b�v
			m_nNumAll++;

			//nCntTex�Ԗڂ�Ԃ�
			return nCntTex;
		}
	}

	if (m_nNumAll > MAX_TEXTURE)
	{// �������e�N�X�`���̍ő吔�𒴂�����
		assert(false);
	}

	// -1��Ԃ�
	return -1;
}

//========================================
//�e�N�X�`���̃A�h���X�擾
//========================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx >= 0 && nIdx <= m_nNumAll)
	{
		return m_apTexture[nIdx];
	}

	//nullptr��Ԃ�
	return nullptr;
}
