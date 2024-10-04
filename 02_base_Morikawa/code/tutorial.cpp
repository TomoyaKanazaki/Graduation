//========================================
//
//�`���[�g���A��[tutorial.h]
//Author�F�X��x��
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "player.h"
#include "field.h"
#include "fade.h"
#include "texture.h"
#include "wall.h"
#include "mapobject.h"
#include "sound.h"

//=======================================
//�R���X�g���N�^
//=======================================
CTutorial::CTutorial()
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CTutorial::~CTutorial()
{
}

//=======================================
//����
//=======================================
CTutorial* CTutorial::Create(void)
{
	//�^�C�g���̃|�C���^
	CTutorial* pTutorial = nullptr;

	//�C���X�^���X����
	pTutorial = new CTutorial;

	//�|�C���^��Ԃ�
	return pTutorial;
}

//=======================================
//������
//=======================================
HRESULT CTutorial::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �C���X�^���X����
	m_pObj2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// �T�C�Y�ݒ�
	m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �e�N�X�`���ݒ�
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\tutorial.png"));

	m_bEnd = false;

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CTutorial::Uninit(void)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);
}

//=======================================
//�X�V
//=======================================
void CTutorial::Update(void)
{
	//CInputKeyboard���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad���擾
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (!m_bEnd &&
		(pInputKeyboard->GetTrigger(DIK_D) ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0)))
	{
		// �e�N�X�`���ݒ�
		m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\tutorial2.png"));
		m_bEnd = true;
		return;
	}

	
	if (m_bEnd && 
		(pInputKeyboard->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0)))
	{
		// ��ʑJ��(�t�F�[�h)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
	}
}

//=======================================
//�`��
//=======================================
void CTutorial::Draw(void)
{
}
