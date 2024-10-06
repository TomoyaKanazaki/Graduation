//==============================================================
//
//�Q�[������[game.cpp]
//Author:����������
//
//==============================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "edit.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "item.h"
#include "pause.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "UI_death.h"
#include "UI_item.h"
#include "wall.h"
#include "tutorial_texture.h"
#include "test.h"

//�ÓI�����o�ϐ��錾
CEdit *CGame::m_pEdit = NULL;				//�G�f�B�b�g�̏��
CPlayer *CGame::m_pPlayer = NULL;			//�v���C���[�̏��
bool CGame::m_bReset = true;				//���Z�b�g�������ǂ���
bool CGame::m_bPause = false;				//�|�[�Y��ʂ�
CPause *CGame::m_pPause = NULL;				//�|�[�Y��ʂ̏��
CScore *CGame::m_pScore = NULL;				//�X�R�A�̏��
bool CGame::m_bEnemySpawnFirst = false;			//�X�^�[�g������
bool CGame::m_bEnemySpawnSecond = false;		//�G���o��������
CDeathUI *CGame::m_pDeathUI = NULL;			//���SUI�̏��
CItemUI *CGame::m_pItemUI = NULL;			//�A�C�e��UI�̏��
CGame::GAMEMODE CGame::m_gameMode = GAMEMODE_START;	//�Q�[�����[�h

//==============================================================
//�R���X�g���N�^
//==============================================================
CGame::CGame()
{
	m_nCntEnemy = 0;				//�G�o���J�E���g

	m_bEnemySpawnFirst = false;			//�G�o��������
	m_bEnemySpawnSecond = false;
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CGame::~CGame()
{

}

//==============================================================
//����������
//==============================================================
HRESULT CGame::Init(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	m_bReset = true;		//���Z�b�g���Ă��Ԃɂ���
	m_bPause = false;		//�|�[�Y���ĂȂ�

	//�J�����̏���������
	pCamera->Init();

	//�ǂ̐���
	//for (int nCntWallWidth = 0; nCntWallWidth < 10; nCntWallWidth++)
	//{//��
	//	for (int nCntWallHeight = 0; nCntWallHeight < 15; nCntWallHeight++)
	//	{//�c

	//		CWall::Create(D3DXVECTOR3(-7000.0f + (nCntWallWidth * 1000.0f), 2000.0f + (nCntWallHeight * -1000.0f), 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//	}
	//}

	// �e�X�g�v���C���[
	CTest::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//�v���C���[�̐���
	//m_pPlayer = m_pPlayer->Create(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

	//�|�[�Y��ʂ̐���
	m_pPause = CPause::Create();

	//BGM�Đ�
	pSound->Play(pSound->SOUND_LABEL_BGM002);

	CObject::SetType(CObject::TYPE_SCENE);

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void CGame::Uninit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//BGM�̒�~
	pSound->Stop();

	//�v���C���[�̔j��
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}

	//�G�f�B�^�[�̔j��
	if (m_pEdit != NULL)
	{
		m_pEdit->Uninit();
		m_pEdit = NULL;
	}

	//�|�[�Y�̔j��
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�X�V����
//==============================================================
void CGame::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//�p�b�h�̏��擾
	CFade *pFade = CManager::GetInstance()->GetFade();			//�t�F�[�h�̏��擾

	if (pInputKeyboard->GetTrigger(DIK_P) == true || 
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_START, 0) == true)
	{//P�L�[����������

		m_bPause = m_bPause ? false : true;		//�|�[�Y�؂�ւ�
	}

	if (m_bPause == true)
	{//�|�[�Y���Ă�Ƃ�

		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_B, 0) == true)
		{
			m_bPause = false;
		}
	}
	else if(m_bPause == false)
	{//�|�[�Y���ĂȂ��Ƃ�

		//�G�o��������
		//CGame::SetEnemy();
	}
}

//==============================================================
//�`�揈��
//==============================================================
void CGame::Draw(void)
{

}

//==============================================================
//�|�[�Y��ʂ̐ݒ菈��
//==============================================================
void CGame::SetEnablePause(bool bPouse)
{
	m_bPause = bPouse;
}

//==============================================================
//�G�o������
//==============================================================
void CGame::SetEnemy(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();
	int nNumSavePoint = m_pPlayer->GetNumSavePoint();		//���Ԗڂ̃Z�[�u�|�C���g���擾

	if (nNumSavePoint == CPlayer::POINT_ENEMYBRIDGE && m_bEnemySpawnFirst == false)
	{//�G���o������ꏊ�ɗ�����

		pSound->Play(pSound->SOUND_LABEL_SE_APPEAR);

		//�G�̐���
		CEnemy::Create(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_bEnemySpawnFirst = true;		//�G�o��������Ԃɂ���
	}
	else if (nNumSavePoint == CPlayer::POINT_BIGINEND && m_bEnemySpawnSecond == false)
	{//�G���o������ꏊ�ɗ�����

		pSound->Play(pSound->SOUND_LABEL_SE_APPEAR);

		//�G�̐���
		CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_bEnemySpawnSecond = true;		//�G�o��������Ԃɂ���
	}

}