//========================================
//
//�^�C�g��[title.cpp]
//Author�F�X��x��
//
//========================================
#include "title.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "player.h"
#include "calculation.h"

//========================================
// �萔��`
//========================================
namespace
{
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";	// �^�C�g���e�N�X�`���̃p�X
	const std::string TEAM = "data\\TEXTURE\\team.png";	// �^�C�g���e�N�X�`���̃p�X
	const std::string PRESS = "data\\TEXTURE\\pressenter.png";	// �^�C�g���e�N�X�`���̃p�X
	const MyLib::Vector3 RANDPOS = MyLib::Vector3(
		UtilFunc::Transformation::Random(0, 13) * 100.0f,
		UtilFunc::Transformation::Random(0, 7) * 100.0f,
		0.0f);
}

//=======================================
//�R���X�g���N�^
//=======================================
CTitle::CTitle(int nPriority)
{
	m_pObj2D = nullptr;
	m_fMoveTime = 0.0f;
}

//=======================================
//�f�X�g���N�^
//=======================================
CTitle::~CTitle()
{
}

//=======================================
//����
//=======================================
CTitle* CTitle::Create(void)
{
	//�^�C�g���̃|�C���^
	CTitle* pTitle = nullptr;

	//�C���X�^���X����
	pTitle = new CTitle;

	//������
	pTitle->Init();

	//�|�C���^��Ԃ�
	return pTitle;
}

//=======================================
//������
//=======================================
HRESULT CTitle::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �C���X�^���X����
	m_pObj2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// �T�C�Y�ݒ�
	m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �e�N�X�`���ݒ�
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\title.png"));

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h�Đ�
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);






	{
		m_pTeam = CObject2D::Create();

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEAM);
		m_pTeam->BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);

		m_pTeam->SetSize(size.x, size.y);
		m_pTeam->SetSizeOrigin(size);

		// �ʒu
		m_pTeam->SetPos(MyLib::Vector3(640.0f, 480.0f, 0.0f));
		m_pTeam->SetOriginPosition(m_pTeam->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}


	{
		m_pPressEnter = CObject2D::Create();

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(PRESS);
		m_pPressEnter->BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);

		m_pPressEnter->SetSize(size.x, size.y);
		m_pPressEnter->SetSizeOrigin(size);

		// �ʒu
		m_pPressEnter->SetPos(MyLib::Vector3(640.0f, 480.0f, 0.0f));
		m_pPressEnter->SetOriginPosition(m_pPressEnter->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CTitle::Uninit(void)
{
}

//=======================================
//�X�V
//=======================================
void CTitle::Update(void)
{
	// �L�[�{�[�h�ƃp�b�h�̃|�C���^
	CInputKeyboard *pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	
	CInputPad *pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();
	
	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();


	m_fMoveTime += CManager::GetInstance()->GetDeltaTime();

	if (m_pPressEnter != nullptr)
	{
		MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_pPressEnter->GetOriginPosition(), m_pPressEnter->GetOldPosition(), 0.0f, 2.0f, m_fMoveTime);
		m_pPressEnter->SetPos(pos);
	}

	if (m_pTeam != nullptr)
	{
		MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_pTeam->GetOriginPosition(), m_pTeam->GetOldPosition(), 0.0f, 2.0f, m_fMoveTime);
		m_pTeam->SetPos(pos);
	}

	if (m_fMoveTime >= 2.0f)
	{
		m_fMoveTime = 0.0f;
		m_pPressEnter->SetOriginPosition(m_pPressEnter->GetPos());
		m_pPressEnter->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));

		m_pTeam->SetOriginPosition(m_pTeam->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}


	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		// ��ʑJ��(�t�F�[�h)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TUTORIAL);
	}
}

//=======================================
//�`��
//=======================================
void CTitle::Draw(void)
{
}
