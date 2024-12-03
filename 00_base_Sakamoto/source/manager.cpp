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
#include "fade.h"
#include "sound.h"
#include "MapSystem.h"
#include "MyEffekseer.h"
#include "Scene.h"

#ifdef _DEBUG
#define SET_MODE (CScene::MODE_GAME)
#define SET_PLAY_MODE (GAME_MODE::MODE_SINGLE)
#else
#define SET_MODE (CScene::MODE_TITLE)
#define SET_PLAY_MODE (GAME_MODE::MODE_SINGLE)
#endif // _DEBUG

//�ÓI�����o�ϐ��錾
CManager* CManager::pManager = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CManager::CManager() :
	m_pEffect(nullptr)
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
	m_pFade = nullptr;
	m_pSound = nullptr;
	m_pRanking = nullptr;
	m_pBlockManager = nullptr;
	m_typeInput = TYPE_INPUT::TYPE_NONE;
	m_nStage = 0;
	m_ScrollType = 0;
	m_End1PScore = 0;
	m_End2PScore = 0;
	m_GameMode = SET_PLAY_MODE;
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
	if (FAILED(m_pSound->Init(hWnd)))
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

	 //�G�t�F�N�g�̐���
	if (m_pEffect == nullptr)
	{
		m_pEffect = new CMyEffekseer;
		m_pEffect->Init();
	}

	// �}�b�v�V�X�e���̏�����
	CMapSystem::GetInstance();
	
	// �e�N�X�`���̏�����	
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

	//�S�Ă�X���f���̓ǂݍ���
	if (FAILED(m_pXModel->Load()))
	{//�ǂݍ��݂����s�����ꍇ
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{
		//�t�F�[�h�̐���
		m_pFade = new CFade;

		if (m_pFade != nullptr)
		{
			m_pFade->Init(SET_MODE);
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
	m_pSound->Stop();

	if (m_pFade != nullptr)
	{
		//�t�F�[�h�̏I������
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pScene != nullptr)
	{
		//�V�[���̏I������
		m_pScene->Uninit();

		m_pScene = nullptr;
	}

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
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = nullptr;
	}

	// �G�t�F�N�V�A�̏I��
	if (m_pEffect != nullptr)
	{
		// �G�t�F�N�V�A�̏I������
		m_pEffect->Uninit();
		delete m_pEffect;
		m_pEffect = nullptr;
	}
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
	(m_pCamera != nullptr) ? m_pCamera->Update() : assert(false);

	//���C�g�̍X�V����
	m_pLight != nullptr ? m_pLight->Update() : assert(false);

	//�L�[�{�[�h�̍X�V����
	m_pInputKeyboard != nullptr ? m_pInputKeyboard->Update() : assert(false);

	//�W���C�p�b�h�̍X�V����
	m_pInputJoyPad != nullptr ? m_pInputJoyPad->Update() : assert(false);

	//�}�E�X�̍X�V����
	m_pInputMouse != nullptr ? m_pInputMouse->Update() : assert(false);

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		if (CScene::GetMode() == CScene::MODE_GAME ||
			CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			//�����H �����P�F�����Q;
			m_bEdit = m_bEdit ? false : true;
			CObject::DeleteBlock();
			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
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
		m_pFade->GetFade() == CFade::FADE_NONE)
	{
		//�����H �����P�F�����Q;
		m_Pause = m_Pause ? false : true;
	}

	//�V�[���̍X�V����
	m_pScene != nullptr ? m_pScene->Update() : assert(false);

	//�����_���[�̍X�V����
	m_pRenderer != nullptr ? m_pRenderer->Update() : assert(false);

	//�t�F�[�h�̍X�V����
	m_pFade != nullptr ? m_pFade->Update() : assert(false);

	// �G�t�F�N�V�A�̍X�V
	m_pEffect != nullptr ? m_pEffect->Update() : assert(false);
}

//====================================================================
//�`�揈��
//====================================================================
void CManager::Draw(void)
{
	//�����_���[�̕`�揈��
	m_pRenderer != nullptr ? m_pRenderer->Draw() : assert(false);
}
