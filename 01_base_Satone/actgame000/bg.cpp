//==============================================================
//
//�w�i����[bg.cpp]
//Author:����������
//
//==============================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define WIDTH_BG		(1280.0f * 0.5f)		//����
#define HEIGHT_BG		(640.0f * 0.5f)			//�c��

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;

//==============================================================
//�R���X�g���N�^
//==============================================================
CBg::CBg()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�

	m_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CBg::~CBg()
{

}

//==============================================================
//��������
//==============================================================
CBg *CBg::Create(void)
{
	CBg *pCBg = NULL;

	if (pCBg == NULL)
	{//���������g�p����ĂȂ�������

		//�w�i�̐���
		pCBg = new CBg;
	}

	//����������
	pCBg->Init();

	return pCBg;
}

//==============================================================
//�w�i�̏���������
//==============================================================
HRESULT CBg::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	int mode = CManager::GetInstance()->GetMode();

	switch (mode)
	{
	case CScene::MODE_TITLE:

		//�e�N�X�`���̓ǂݍ���
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\title.jpg");

		break;

	case CScene::MODE_TUTORIAL:

		//�e�N�X�`���̓ǂݍ���
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\tutorial.jpg");

		break;

	case CScene::MODE_RESULT:

		//�e�N�X�`���̓ǂݍ���
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\result.jpg");

		break;

	case CScene::MODE_RANKING:

		//�e�N�X�`���̓ǂݍ���
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\ranking.jpg");

		break;
	}

	//�e�N�X�`���̊��蓖��
	CObject2D::BindTexture(m_nIdxTexture);

	//�I�u�W�F�N�g2D�̏���������
	CObject2D::Init();

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_NONE);

	///�w�i�̏����ݒ�
	//�w�i�̑傫���ݒ�
	CObject2D::SetSize(WIDTH_BG, HEIGHT_BG);

	//�ʒu�ݒ�
	CObject2D::SetPosition(m_pos);

	return S_OK;
}

//==============================================================
//�w�i�̏I������
//==============================================================
void CBg::Uninit(void)
{
	//2D�I�u�W�F�N�g�̏I������
	CObject2D::Uninit();
}

//==============================================================
//�w�i�̍X�V����
//==============================================================
void CBg::Update(void)
{
	//2D�I�u�W�F�N�g�̍X�V����
	CObject2D::Update();
}

//==============================================================
//�w�i�̕`�揈��
//==============================================================
void CBg::Draw(void)
{
	//2D�I�u�W�F�N�g�̍X�V����
	CObject2D::Draw();
}