//========================================
//
//�Q�[��������[game.cpp]
//Author�F�X��x��
//
//========================================
#include "game.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "sound.h"
#include "texture.h"
#include "fade.h"
#include "object2D.h"

#include "timer.h"
#include "field.h"
#include "sky.h"
#include "stageobj.h"
#include "stagemanager.h"
#include "goal.h"
#ifdef _DEBUG
#include "stageEdittor.h"
#endif // _DEBUG


//========================================
//�ÓI�����o�ϐ�
//========================================
CGame *CGame::m_pGame = nullptr;			// �Q�[���̃|�C���^

//========================================
// �萔��`
//========================================
namespace
{
const int TRANSITIONTIME = 120;		// �J�ڂ���܂ł̎���

const char* PLAYER_PASS = "data\\FILE\\turtle.txt"; // �v���C���[�̃p�X
}

//========================================
//�R���X�g���N�^
//========================================
CGame::CGame() : 
	m_bPause(false),
	m_pFade(nullptr),
	m_pTimer(nullptr),
	m_Obj2D(nullptr),
	m_bOver(false),
	m_bClear(false),
	m_pStageManager(nullptr)
#ifdef _DEBUG
	,m_pEdittor(nullptr)		// �G�f�B�^�[
#endif // _DEBUG

{
	m_pGame = nullptr;
}

//========================================
//�f�X�g���N�^
//========================================
CGame::~CGame()
{
}

//========================================
//����
//========================================
CGame *CGame::Create(void)
{
	if (m_pGame == nullptr)
	{
		//�C���X�^���X����
		m_pGame = new CGame;
	}

	//�|�C���^��Ԃ�
	return m_pGame;
}

//========================================
//������
//========================================
HRESULT CGame::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	CField::Create();
	CSky::Create();

	// �v���C���[�̐���
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create(PLAYER_PASS);
	}

	// �^�C�}�[
	if (m_pTimer == nullptr)
	{
		m_pTimer = CTimer::Create();
	}

	// �X�e�[�W�}�l�[�W���[�̐���
	if (m_pStageManager == nullptr)
	{
		m_pStageManager = CStageManager::Create();
	}

	// �S�[���̐ݒu
	CGoal::Create();

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h�Đ�
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//========================================
//�I��
//========================================
void CGame::Uninit(void)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop();

	if(m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	if (m_pStageManager != nullptr)
	{
		m_pStageManager->Uninit();
		m_pStageManager = nullptr;
	}

	if (m_Obj2D != nullptr)
	{
		m_Obj2D->Uninit();
		m_Obj2D = nullptr;
	}

#ifdef _DEBUG

	if (m_pEdittor != nullptr)
	{
		m_pEdittor->Uninit();
		m_pEdittor = nullptr;
	}

#endif // _DEBUG

	m_pGame = nullptr;
}

//========================================
//�X�V
//========================================
void CGame::Update(void)
{
	//�|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �^�C�}�[���̗͂�0����������ꍇ�I��
	if ((m_pTimer->GetTimeZero() || CPlayer::GetInstance()->GetLife() <= 0.0f) && !m_bClear)
	{
		// �^�C�g���ɑJ��
		m_bOver = true;

		if (m_Obj2D == nullptr)
		{
			// �C���X�^���X����
			m_Obj2D = CObject2D::Create();

			// �ʒu�ݒ�
			m_Obj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

			// �T�C�Y�ݒ�
			m_Obj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

			// �e�N�X�`���ݒ�
			m_Obj2D->BindTexture(pTexture->Regist("data\\texture\\over.png"));
		}
	}

	// �S�[��
	if (CPlayer::GetInstance()->GetPos().x >= 50000.0f && !m_bOver)
	{
		// ���U���g�ɑJ��
		m_bClear = true;

		if (m_Obj2D == nullptr)
		{
			// �C���X�^���X����
			m_Obj2D = CObject2D::Create();

			// �ʒu�ݒ�
			m_Obj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

			// �T�C�Y�ݒ�
			m_Obj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

			// �e�N�X�`���ݒ�
			m_Obj2D->BindTexture(pTexture->Regist("data\\texture\\clear.png"));
		}
	}

	if (m_bOver || m_bClear)
	{
		++m_nTransition;

		if (m_nTransition >= TRANSITIONTIME)
		{
			if (m_bOver)
			{
				CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_TITLE);
			}
			if (m_bClear)
			{
				CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_RESULT);
			}
		}
	}

#ifdef _DEBUG

	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{// �Q�[����ʂɑJ��
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
	
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		CStageObj::CreateReverse(MyLib::Vector3(500.0f, 500.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		if (m_pEdittor == nullptr)
		{
			m_pEdittor = CStageEdittor::Create();
		}
		else
		{
			m_pEdittor->Uninit();
			m_pEdittor = nullptr;
		}
	}

	if (m_pEdittor != nullptr)
	{
		// �J�����̒Ǐ]�ݒ�
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->Following(m_pEdittor->GetObj()->GetPos(), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}
#endif
}

//========================================
//�`��
//========================================
void CGame::Draw(void)
{
}
