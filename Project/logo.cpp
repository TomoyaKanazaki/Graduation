//============================================
//
//	���S��� [logo.cpp]
//	Author : Takeru Ogasawara
//
//============================================

#include "logo.h"
#include "object2D.h"

// �萔��`
namespace
{
	const int WAIT = 120;	// ���S�̑ҋ@����
	const float DECLEASE = 0.01f;	// ���S�̌������x
}

// �ÓI�����o�ϐ��錾
CObject2D* CLogo::m_pLogo = nullptr;
CObject2D* CLogo::m_pBg = nullptr;
int CLogo::m_nCounter = 0;
bool CLogo::m_bStart = false;

//====================================================================
// �R���X�g���N�^
//====================================================================
CLogo::CLogo()
{
	m_pLogo = nullptr;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CLogo::~CLogo()
{

}

//====================================================================
// ����������
//====================================================================
HRESULT CLogo::Init(void)
{
	//// �w�i�̐���
	//m_pBg = CObject2D::Create();

	//if (m_pBg)
	//{
	//	m_pBg->SetPos({ 640.0f, 360.0f, 0.0f });
	//	m_pBg->SetWidth(1280.0f);
	//	m_pBg->SetHeight(720.0f);

	//}

	// ���S�̐���
	m_pLogo = CObject2D::Create();

	if (m_pLogo)
	{
		m_pLogo->SetTexture("data\\TEXTURE\\logo\\logo.jpg");
		m_pLogo->SetPos({ 640.0f, 360.0f, 0.0f });
		m_pLogo->SetWidth(500.0f);
		m_pLogo->SetHeight(500.0f);
	}

	m_nCounter = WAIT;

	return S_OK;
}

//====================================================================
// �I������
//====================================================================
void CLogo::Uninit(void)
{

}

//====================================================================
// �X�V����
//====================================================================
void CLogo::Update(void)
{
	if (m_nCounter < 0)
	{
		m_bStart = true;
	}

	if (!m_bStart)
	{
		m_nCounter -= (int)CManager::GetInstance()->GetGameSpeed();

		return;
	}

	if (UIFade())
	{
		CScene::SetMode(CScene::MODE_TITLE);
	}
}

//====================================================================
// �`�揈��
//====================================================================
void CLogo::Draw(void)
{

}

//====================================================================
// UI�t�F�[�h
//====================================================================
bool CLogo::UIFade(void)
{
	float fColorA = 0.0f;

	// ���S�̐F(���l)�̎擾
	fColorA = m_pLogo->GetColor().a;

	// �F(���l)�̌���
	fColorA -= DECLEASE;

	// �F�̐ݒ�
	m_pLogo->SetColorA(fColorA);

	if (fColorA < 0.0f)
	{
		fColorA = 0.0f;

		// �F(���l)��0�ɂȂ������Ƃ�Ԃ�
		return true;
	}

	return false;
}
