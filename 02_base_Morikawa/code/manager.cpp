//========================================
//
// �Ǘ�[manager.cpp]
// Author�F�X��x��
//
//========================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "XLoad.h"
#include "sound.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CManager *CManager::m_pManager = nullptr;

//========================================
//�R���X�g���N�^
//========================================
CManager::CManager(void) : 
	m_nCnt(0),	//�����J�ڂ̃J�E���^�[
	m_state(0),	//��ԕύX
	m_NowScene(CScene::MODE_NONE),	//���݂̃V�[��
	m_pRenderer(nullptr),				// �����_���[�̃|�C���^
	m_pInputKeyboard(nullptr),
	m_pInputPad(nullptr),
	m_pInputMouse(nullptr),
	m_pScene(nullptr),
	m_pTexture(nullptr),
	m_pCamera(nullptr),
	m_pLight(nullptr),
	m_pFade(nullptr),
	m_pSound(nullptr),
	m_CurrentTime(0),
	m_OldTime(0),
	m_fDeltaTime(0.0f)
{//�l�N���A
}

//========================================
//�f�X�g���N�^
//========================================
CManager::~CManager(void)
{
}

//========================================
// �}�l�[�W���[�̏��擾
//========================================
CManager *CManager::GetInstance()
{
	if (m_pManager == nullptr)
	{//�C���X�^���X����
		return m_pManager = new CManager;
	}
	else
	{//�|�C���^��Ԃ�
		return m_pManager;
	}
}

//========================================
//������
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_���[�̐���
	if (m_pRenderer == nullptr)
	{//g_pRenderer��nullptr�̎�
		m_pRenderer = new CRenderer;

		//�����_���[�̏���������
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{//���������������s�����ꍇ
			return -1;
		}
	}

	//�L�[�{�[�h�̐���
	if (m_pInputKeyboard == nullptr)
	{//m_pInputKeyboard��nullptr�̎�
		m_pInputKeyboard = new CInputKeyboard;

		//�L�[�{�[�h�̏���������
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ
			return -1;
		}
	}

	//�R���g���[���[�̐���
	if (m_pInputPad == nullptr)
	{//m_pInputKeyboard��nullptr�̎�
		m_pInputPad = new CInputPad;

		//�R���g���[���[�̏���������
		if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ
			return -1;
		}
	}

	//�J��������
	if (m_pCamera == nullptr)
	{//m_pCamera��nullptr�̎�
		m_pCamera = new CCamera;

		//�J�����̏���������
		if (FAILED(m_pCamera->Init()))
		{//���������������s�����ꍇ
			return -1;
		}
	}

	//debugproc�̐���
	DebugProc::Init();

	if (m_pLight == nullptr)
	{// m_pLight��nullptr�̎�
		m_pLight = new CLight;

		if (FAILED(m_pLight->Init()))
		{// ���������s
			return -1;
		}
	}

	if (m_pSound == nullptr)
	{
		// �C���X�^���X����
		m_pSound = new CSound;

		if (FAILED(m_pSound->Init(hWnd)))
		{// ���������s
			return -1;
		}

	}

	//**********************************
	// X�t�@�C��
	//**********************************
	m_pXLoad = CXLoad::Create();
	if (m_pXLoad == nullptr)
	{
		return E_FAIL;
	}

	//�e�N�X�`������
	m_pTexture = CTexture::Create();

	if (m_pFade == nullptr)
	{
		// �t�F�[�h�����E�ݒ�
		m_pFade = CFade::Create(CScene::MODE_TITLE);
		m_pFade->SetFade(CScene::MODE_TITLE);
	}

#ifdef _DEBUG
	{
		m_pFade = CFade::Create(CScene::MODE_GAME);
		m_pFade->SetFade(CScene::MODE_GAME);
	}
#endif

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CManager::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
	
	if (m_pRenderer != nullptr)
	{//�����_���[�̏I��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{//�L�[�{�[�h�̔j��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputPad != nullptr)
	{//�R���g���[���[�̔j��
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = nullptr;
	}

	//debugproc�̔j��
	DebugProc::Uninit();

	if (m_pTexture != nullptr)
	{//�e�N�X�`���j��
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pFade != nullptr)
	{// �t�F�[�h�I��
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pSound != nullptr)
	{// �T�E���h�I��
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//========================================
//�X�V
//========================================
void CManager::Update(void)
{
	// �ߋ��̎��ԕۑ�
	m_OldTime = m_CurrentTime;

	// �o�ߎ���
	m_CurrentTime = timeGetTime();
	m_fDeltaTime = static_cast<float>(m_CurrentTime - m_OldTime) / 1000;

	if (m_pRenderer != nullptr)
	{//g_pRenderer��nullptr�̎�
		m_pRenderer->Update();
	}
	
	if (m_pInputKeyboard != nullptr)
	{//�L�[�{�[�h�̍X�V
		m_pInputKeyboard->Update();
	}
	
	if (m_pInputPad != nullptr)
	{//�R���g���[���[�̍X�V
		m_pInputPad->Update();
	}
	
	//DebugPro�̍X�V
	DebugProc::Update();
	
	if (m_pScene != nullptr)
	{//�V�[���̍X�V
		m_pScene->Update();
	}

	if (m_pFade != nullptr)
	{// �t�F�[�h�X�V
		m_pFade->Update();
	}

	if (m_pCamera != nullptr)
	{// �J�����X�V
		m_pCamera->Update();
	}
}

//========================================
//�`��
//========================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{//g_pRenderer��nullptr�̎�
		//�`�揈��
		m_pRenderer->Draw();
	}

	if (m_pScene != nullptr)
	{//�V�[���̍X�V
		m_pScene->Draw();
	}
}

//========================================
//���[�h�ݒ�
//========================================
void CManager::SetMode(CScene::MODE mode)
{	
	if (m_pScene != nullptr)
	{//���[�h�j��
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//�S�Ĕj��
	CObject::ReleaseAll();

	//���[�h����
	m_pScene = CScene::Create(mode);
}

//========================================
// ���s����
//========================================
bool CManager::bResult()
{
	return m_IsResult;
}

//========================================
//�R���X�g���N�^
//========================================
CScene::CScene() : 
	m_pVtxBuff(nullptr),	//���_�����i�[
	m_nIdxTexture(0),		//�e�N�X�`���ԍ�
	m_mode(MODE_NONE)		//���[�h
{//�l�N���A
}

//========================================
//�f�X�g���N�^
//========================================
CScene::~CScene()
{
}

//========================================
//����
//========================================
CScene* CScene::Create(int nMode)
{
	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();;

	//CScene�̃|�C���^
	CScene* pScene = nullptr;

	switch (nMode)
	{
	case MODE_TITLE:
		// �^�C�g������
		pScene = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		// �`���[�g���A������
		pScene = CTutorial::Create();
		break;

	case MODE_GAME:
		// �Q�[���V�[������
		pScene = CGame::Create();
		break;

	case MODE_RESULT:
		// ���U���g����
		pScene = CResult::Create();
		break;
	}

	if (pScene != nullptr)
	{
		// �V�[���̏�����
		pScene->Init();

		// �J�����̏�����
		pCamera->Init();
	}

	//�|�C���^��Ԃ�
	return pScene;
}
 