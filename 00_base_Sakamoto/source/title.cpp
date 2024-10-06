//============================================
//
//	�^�C�g����� [title.cpp]
//	Author:sakamoto kai
//
//============================================
#include "title.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
#include "particle.h"
#include "sound.h"
#include "objmeshDome.h"
#include "player.h"
#include "CubeBlock.h"
#include "2DUI_edit.h"
#include "objmeshDome.h"
#include "objmeshField.h"
#include "camera.h"
//#include "cursorUI.h"
#include "useful.h"
#include "MapModel.h"
#include "light.h"
#include "sound.h"

// �萔��`
namespace
{
	
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");
	
	const float FADE_WAIT_TIME = 120.0f;	// UI��������܂ł̎���
	const float FADE_SPEED = 0.01f;	// UI�������鑬��

	const float SLASH_SPEED = 3.5f;	// �a�ꂽ��̗����鑬�x
	const float SLASH_POS_LIMIT = 40.0f;	// UI���a������ʒu

	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 60.0f };	// �{�^���̑傫��

	// �a���Ɋւ���
	const D3DXVECTOR3 SLASH_POS = { 610.0f, 310.0f, 0.0f };	// �ʒu
	const float SLASH_LENGTH = 350.0f;		// ����
	const float SLASH_ANGLE = 2.93f;		// �p�x
	const D3DXVECTOR2 SLASH_SIZE_DEFAULT = D3DXVECTOR2(SLASH_LENGTH * 0.5f, SLASH_LENGTH * 0.25f);	// �傫���̃f�t�H���g
	const D3DXVECTOR2 SLASH_SIZE_DEST_2D = D3DXVECTOR2(0.0f, 30.0f);	// �傫���̌�����
	const int EVENT_SPWAN_SLASH_DIV = 3;	// ���̃X���b�V���R�}�܂ł̏o���t���[����
	const int EVENT_DEST_START = 1;		// �傫���̌����J�n�̃t���[��
	const int EVENT_END = 60;				// �I���̃t���[��

	const float DOME_ROT_SPEED = 0.001f;	// ���b�V���h�[���̉�]���x
}

//�ÓI�����o�ϐ��錾
CObject2D* CTitle::m_pLogo = nullptr;
CObject2D *CTitle::m_pTitleButton = nullptr;
CObjmeshDome* CTitle::m_pMeshDome = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CTitle::CTitle()
{
	ZeroMemory(&m_pInfoSlash, sizeof(m_pInfoSlash));
	m_fCountFade = 0.0f;
	m_bStart = false;
	m_bTurn = false;
	m_bSlash = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CTitle::~CTitle()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �X�J�C�h�[��
	m_pMeshDome = CObjmeshDome::Create();
	if (m_pMeshDome)
	{
		m_pMeshDome->SetTexture("data\\TEXTURE\\sky.jpg");
	}

	CObjmeshField* pMeshField = CObjmeshField::Create(21, 21);
	pMeshField->SetPos(INITVECTOR3);
	pMeshField->SetTexture("data\\TEXTURE\\field00.jpg");

	// �^�C�g������
	m_pLogo = CObject2D::Create();
	m_pLogo->SetPos(D3DXVECTOR3(640.0f, 241.0f, 0.0f));
	m_pLogo->SetWidth(1000.0f);
	m_pLogo->SetHeight(400.0f);
	if (pTexture)
	{
		m_pLogo->SetIdx(pTexture->Regist("data\\TEXTURE\\title\\logo001.png"));
	}

	// �{�^��
	m_pTitleButton = CObject2D::Create();
	m_pTitleButton->SetPos(D3DXVECTOR3(640.0f, 670.0f, 0.0f));
	m_pTitleButton->SetWidth(BUTTON_SIZE.x);
	m_pTitleButton->SetHeight(BUTTON_SIZE.y);
	//m_pTitleButton->SetColorA(0.2f);
	if (pTexture)
	{
		m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\title\\titleButton.png"));
	}

	// �J�[�\���̐���
	//m_pCorsorUI = CCursorUI::Create();

	// �t�F�[�h�J�E���g�ݒ�
	m_fCountFade = FADE_WAIT_TIME;
	
	// ���C�g�̏�����
	CManager::GetInstance()->GetLight()->Init();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CTitle::Uninit(void)
{
	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//====================================================================
//�X�V����
//====================================================================
void CTitle::Update(void)
{	
	// �I������
	Select();
}

//====================================================================
//�`�揈��
//====================================================================
void CTitle::Draw(void)
{

}

//====================================================================
//�I������
//====================================================================
void CTitle::Select(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// ���̓^�C�v�F�L�[�}�E
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

		// �Q�[���J��
		CFade::SetFade(CScene::MODE_GAME);
	}
	else if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_JOYPAD);	// ���̓^�C�v�F�p�b�h
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

		// �Q�[���J��
		CFade::SetFade(CScene::MODE_GAME);
	}
}