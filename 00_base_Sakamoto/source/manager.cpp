//============================================
//
//	�}�l�[�W�� [manager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "XModel.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "tutorial.h"
#include "logo.h"
#include "MapSystem.h"
#include "MyEffekseer.h"

#ifdef _DEBUG
#define SET_MODE (CScene::MODE_GAME)
#else
#define SET_MODE (CScene::MODE_GAME)
#endif // _DEBUG

//�ÓI�����o�ϐ��錾
CManager* CManager::pManager = nullptr;
CScene::MODE CScene::m_mode = SET_MODE;

//====================================================================
//�R���X�g���N�^
//====================================================================
CManager::CManager()
{
	//�ÓI�����o�ϐ��錾
	m_bEdit = false;
	m_bStop = false;
	m_SetTutorial = false;
	m_Pause = false;
	m_SetScoreResult = false;
	m_SetJoyPad = false;
	m_PauseOK = false;
	m_bPad = false;
	m_EndScore = 0;
	m_bGameClear = false;
	m_bState = true;
	pManager = nullptr;
	m_pRenderer = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputJoyPad = nullptr;
	m_pInputMouse = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pTexture = nullptr;
	m_pXModel = nullptr;
	m_pScene = nullptr;
	m_LevelUP = nullptr;
	m_Fade = nullptr;
	m_pSound = nullptr;
	m_pRanking = nullptr;
	m_pBlockManager = nullptr;
	m_typeInput = TYPE_INPUT::TYPE_NONE;
	m_nStage = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CManager::~CManager()
{

}

//====================================================================
//�C���X�^���X�擾
//====================================================================
CManager *CManager::GetInstance(void)
{
	if (pManager == nullptr)
	{
		pManager = new CManager;
	}
	return pManager;
}

//====================================================================
//����������
//====================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	GameSpeed = 1.00000000f;

	if (m_pRenderer == nullptr)
	{
		//�����_���[�̐���
		m_pRenderer = new CRenderer;
	}

	//�����_���[�̏���������
	if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pSound == nullptr)
	{
		//�T�E���h�̐���
		m_pSound = new CSound;
	}

	//�T�E���h�̏���������
	if (FAILED(m_pSound->InitSound(hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pInputKeyboard == nullptr)
	{
		//�L�[�{�[�h�̐���
		m_pInputKeyboard = new CInputKeyboard;
	}
	//�L�[�{�[�h�̏���������
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pInputJoyPad == nullptr)
	{
		//�W���C�p�b�h�̐���
		m_pInputJoyPad = new CInputJoypad;
	}
	//�W���C�p�b�h�̏���������
	if (FAILED(m_pInputJoyPad->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pInputMouse == nullptr)
	{
		//�}�E�X�̐���
		m_pInputMouse = new CInputMouse;
	}
	//�L�[�{�[�h�̏���������
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pCamera == nullptr)
	{
		//�J�����̐���
		m_pCamera = new CCamera;
	}

	//�J�����̏���������
	if (FAILED(m_pCamera->Init()))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	if (m_pLight == nullptr)
	{
		//���C�g�̐���
		m_pLight = new CLight;
	}

	//���C�g�̏���������
	if (FAILED(m_pLight->Init()))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	CMapSystem::GetInstance()->Init();

	if (m_pTexture == nullptr)
	{
		//�e�N�X�`���̐���
		m_pTexture = new CTexture;
	}

	//�S�Ẵe�N�X�`���̓ǂݍ���
	if (FAILED(m_pTexture->Load()))
	{//�ǂݍ��݂����s�����ꍇ
		return E_FAIL;
	}


	if (m_pXModel == nullptr)
	{
		//X���f���̐���
		m_pXModel = new CXModel;
	}

	////�A�C�e���}�l�[�W���̓ǂݍ��ݏ���
	//CItemManager::Load();

	//�S�Ă�X���f���̓ǂݍ���
	if (FAILED(m_pXModel->Load()))
	{//�ǂݍ��݂����s�����ꍇ
		return E_FAIL;
	}

	if (m_Fade == nullptr)
	{
		//�t�F�[�h�̐���
		m_Fade = new CFade;

		if (m_Fade != nullptr)
		{
			m_Fade->Init(SET_MODE);
		}
	}

	m_PauseOK = true;

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CManager::Uninit(void)
{
	//BGM�̒�~
	m_pSound->StopSound();

	if (m_Fade != nullptr)
	{
		//�t�F�[�h�̏I������
		m_Fade->Uninit();

		delete m_Fade;
		m_Fade = nullptr;
	}

	if (m_pScene != nullptr)
	{
		//�V�[���̏I������
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	////�A�C�e���}�l�[�W���̓ǂݍ��ݏ���
	//CItemManager::UnLoad();

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	CMapSystem::GetInstance()->Uninit();

	if (m_pTexture != nullptr)
	{
		//�e�N�X�`���̏I������
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pXModel != nullptr)
	{
		//X���f���̏I������
		m_pXModel->Unload();

		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pLight != nullptr)
	{
		//���C�g�̏I������
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pCamera != nullptr)
	{
		//�J�����̏I������
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{
		//�W���C�p�b�h�̏I������
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	if (m_pInputJoyPad != nullptr)
	{
		//�W���C�p�b�h�̏I������
		m_pInputJoyPad->Uninit();

		delete m_pInputJoyPad;
		m_pInputJoyPad = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{
		//�L�[�{�[�h�̏I������
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		//�����_���[�̏I������
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pSound != nullptr)
	{
		//�T�E���h�̏I������
		m_pSound->UninitSound();

		delete m_pSound;
		m_pSound = nullptr;
	}

	// �G�t�F�N�V�A�̏I��
	CMyEffekseer::GetInstance()->Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CManager::Update(void)
{
	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "-----�f�o�b�O�\��-----\n");
	DebugProc::Print(DebugProc::POINT_CENTER, "FPS : %d\n", GetFps());

	//�J�����̍X�V����
	m_pCamera->Update();

	//���C�g�̍X�V����
	m_pLight->Update();

	//�L�[�{�[�h�̍X�V����
	m_pInputKeyboard->Update();

	//�W���C�p�b�h�̍X�V����
	m_pInputJoyPad->Update();

	//�}�E�X�̍X�V����
	m_pInputMouse->Update();

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		if (CScene::GetMode() == CScene::MODE_GAME ||
			CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			//�����H �����P�F�����Q;
			m_bEdit = m_bEdit ? false : true;
			CObject::DeleteBlock();
		}
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		bool Set = CManager::GetInstance()->GetRenderer()->GetAfterImage();
		Set = !Set;
		CManager::GetInstance()->GetRenderer()->SetAfterImage(Set);
	}

#endif

	if ((CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_START, 0) == true) &&
		m_PauseOK == true &&
		m_Fade->GetFade() == CFade::FADE_NONE)
	{
		//�����H �����P�F�����Q;
		m_Pause = m_Pause ? false : true;
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_DASH);
	}

	//�V�[���̍X�V����
	m_pScene->Update();

	//�����_���[�̍X�V����
	m_pRenderer->Update();

	//�t�F�[�h�̍X�V����
	m_Fade->Update();

	// �G�t�F�N�V�A�̍X�V
	CMyEffekseer::GetInstance()->Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CManager::Draw(void)
{
	//�����_���[�̕`�揈��
	m_pRenderer->Draw();
}

//====================================================================
//��������
//====================================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	if (pScene == nullptr)
	{
		//�V�[���̐���
		switch (mode)
		{
		case MODE_LOGO:
			pScene = new CLogo();
			break;
		case MODE_TITLE:
			pScene = new CTitle();
			break;
		case MODE_GAME:
			pScene = new CGame();
			break;
		case MODE_RESULT:
			pScene = new CResult();
			break;
		case CScene::MODE_TUTORIAL:
			pScene = new CTutorial();
			break;
		}
	}

	CManager::GetInstance()->GetInstance()->GetCamera()->ResetCamera();

	CManager::GetInstance()->GetInstance()->SetEdit(false);

	m_mode = mode;

	if (m_mode == MODE_GAME)
	{

	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pScene->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pScene;
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CScene::CScene()
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CScene::~CScene()
{
}

//====================================================================
//����������
//====================================================================
HRESULT CScene::Init(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Init();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CScene::Uninit(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CScene::Update(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CScene::Draw(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Draw();
}

//====================================================================
//���[�h�ݒ菈��
//====================================================================
void CScene::SetMode(MODE mode)
{
	CScene *pScene = CManager::GetInstance()->GetScene();
	CManager::GetInstance()->GetSound()->StopSound();

	if (pScene != nullptr)
	{
		//�V�[���̏I������
		pScene->Uninit();

		delete pScene;
		pScene = nullptr;
	}

	//���[�h�̐���
	pScene = Create(mode);

	CManager::GetInstance()->GetInstance()->SetScene(pScene);
}