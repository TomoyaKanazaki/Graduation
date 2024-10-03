//==============================================================
//
//�}�l�[�W������[manager.cpp]
//Author:����������
//
//==============================================================
#include "manager.h"
#include "object.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "scene.h"
#include "texture.h"
#include "fade.h"
#include "material.h"
#include "camera.h"
#include "light.h"
//#include "model.h"

//�ÓI�����o�ϐ��錾
CManager *CManager::m_pManager = NULL;			//�}�l�[�W���̏��

CRenderer *CManager::m_pRenderer = NULL;			//�����_���[�̏��
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	//�L�[�{�[�h�̏��
CInputJoyPad *CManager::m_pInputJoyPad = NULL;		//�Q�[���p�b�h�̏��
CDebugProc *CManager::m_pDebugProc = NULL;			//�f�o�b�O�\���̏��
CSound *CManager::m_pSound = NULL;					//�T�E���h�̏��
CScene *CManager::m_pScene = NULL;					//�V�[���̏��
CTexture *CManager::m_pTexture = NULL;				//�e�N�X�`���̏��
CMaterial *CManager::m_pMaterial = NULL;			//�}�e���A���̏��
CFade *CManager::m_pFade = NULL;					//�t�F�[�h�̏��

CCamera *CManager::m_pCamera = NULL;				//�J�����̏��
CLight *CManager::m_pLight = NULL;					//���C�g�̏��

//CPlayer *CManager::m_pPlayer = NULL;			//�v���C���[�̏��

int CManager::m_nScore = 0;						//�X�R�A
int CManager::m_nNumDeath = 0;					//���S��
int CManager::m_nNumItem = 0;					//�A�C�e����
bool CManager::m_bBgm = false;					//BGM�X�g�b�v���邩
bool CManager::m_bResult = true;				//���U���g�̔���

//==============================================================
//�}�l�[�W���̃R���X�g���N�^
//==============================================================
CManager::CManager()
{

}

//==============================================================
//�}�l�[�W���̃f�X�g���N�^
//==============================================================
CManager::~CManager()
{

}

//==============================================================
//�}�l�[�W���̏���������
//==============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_���[�̐���
	if (m_pRenderer == NULL)
	{//���������g�p����ĂȂ��Ƃ�

		m_pRenderer = new CRenderer;
	}

	//�����_���[�̏���������
	if (m_pRenderer != NULL)
	{//�������m�ۂł��Ă���

		//�����_���[�̏�����
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{//���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//�f�o�b�O�\���̐���
	if (m_pDebugProc == NULL)
	{//���������g�p����ĂȂ��Ƃ�

		m_pDebugProc = new CDebugProc;
	}

	if (m_pDebugProc != NULL)
	{//���������m�ۂ��ꂽ�Ƃ�

		//�f�o�b�O�\���̏���������
		m_pDebugProc->Init();
	}

	//�L�[�{�[�h�̐���
	if (m_pInputKeyboard == NULL)
	{//�L�[�{�[�h���g�p����ĂȂ��Ƃ�

		m_pInputKeyboard = new CInputKeyboard;
	}

	if (m_pInputKeyboard != NULL)
	{//�������m�ۂł��Ă���

		//�L�[�{�[�h�̏�����
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//�Q�[���p�b�h�̐���
	if (m_pInputJoyPad == NULL)
	{
		m_pInputJoyPad = new CInputJoyPad;
	}

	if (m_pInputJoyPad != NULL)
	{//�������m�ۂł��Ă���

		//�p�b�h�̏�����
		if (FAILED(m_pInputJoyPad->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//�T�E���h�̐���
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
	}

	if (m_pSound != NULL)
	{//�������m�ۂł��Ă���

		//�T�E���h�̏�����
		if (FAILED(m_pSound->Init(hWnd)))
		{//���������������s�����ꍇ
			return E_FAIL;
		}
	}

	//�J�����̐���
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
	}

	if (m_pCamera != NULL)
	{//�������m�ۂł��Ă���

		//�J�����̏�����
		if (FAILED(m_pCamera->Init()))
		{//���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//���C�g�̐���
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
	}

	if (m_pLight != NULL)
	{//�������m�ۂł��Ă���

		//���C�g�̏�����
		if (FAILED(m_pLight->Init()))
		{//���������������s�����ꍇ

			return E_FAIL;
		}
	}

	//�e�N�X�`���̐���
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
	}

	if (m_pTexture != NULL)
	{//�������m�ۂł��Ă���

		//�e�N�X�`���̓ǂݍ���
		if (FAILED(m_pTexture->Load()))
		{//�ǂݍ��݂����s�����ꍇ

			return E_FAIL;
		}
	}

	//�}�e���A���̐���
	if (m_pMaterial == NULL)
	{
		m_pMaterial = new CMaterial;
	}

	if (m_pMaterial != NULL)
	{//�������m�ۂł��Ă���

		//�}�e���A���̓ǂݍ���
		if (FAILED(m_pMaterial->Load()))
		{//�ǂݍ��݂����s�����ꍇ

			return E_FAIL;
		}
	}

	//�V�[���̐�������
	if (m_pScene == NULL)
	{
#if _DEBUG

		m_pScene = CScene::Create(CScene::MODE_GAME);

#else

		m_pScene = CScene::Create(CScene::MODE_TITLE);

#endif
	}

	if (m_pScene != NULL)
	{//�������m�ۂł�����

		//�V�[���̏���������
		if (FAILED(m_pScene->Init()))
		{//�ǂݍ��݂����s�����ꍇ

			return E_FAIL;
		}
	}

	//�t�F�[�h�̐�������
	if (m_pFade == NULL)
	{
		m_pFade = CFade::Create();
	}
	
	return S_OK;
}

//==============================================================
//�}�l�[�W���̏I������
//==============================================================
void CManager::Uninit(void)
{
	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	//�S�Ẵ}�e���A���̔j��
	if (m_pMaterial != NULL)
	{
		//�e�N�X�`���̏I������
		m_pMaterial->Unload();

		delete m_pMaterial;
		m_pMaterial = NULL;
	}

	//�S�Ẵe�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		//�e�N�X�`���̏I������
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	//�V�[���̔j��
	if (m_pScene != NULL)
	{
		m_pScene = NULL;
	}

	//���C�g�̔j��
	if (m_pLight != NULL)
	{
		//���C�g�̏I������
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	//�J�����̔j��
	if (m_pCamera != NULL)
	{
		//�J�����̏I������
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�T�E���h�̔j��
	if (m_pSound != NULL)
	{
		//�T�E���h�̏I������
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	//�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		//�L�[�{�[�h�̏I������
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�Q�[���p�b�h�̔j��
	if (m_pInputJoyPad != NULL)
	{
		//�Q�[���p�b�h�̏I������
		m_pInputJoyPad->Uninit();

		delete m_pInputJoyPad;
		m_pInputJoyPad = NULL;
	}

	//�f�o�b�O�\���̔j��
	if (m_pDebugProc != NULL)
	{
		//�f�o�b�O�\���̏I������
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		//�����_���[�̏I������
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�}�l�[�W���̔j��
	if (m_pManager != NULL)
	{
		delete m_pManager;
		m_pManager = NULL;
	}
}

//==============================================================
//�}�l�[�W���̍X�V����
//==============================================================
void CManager::Update(void)
{
	//�L�[�{�[�h�̍X�V����
	m_pInputKeyboard->Update();

	//�Q�[���p�b�h�̍X�V����
	m_pInputJoyPad->Update();

	//�f�o�b�O�\���̍X�V����
	m_pDebugProc->Update();

	//�}�b�v�̍Đ���
	CManager::ResetMap();

	//�����_���[�̍X�V����
	m_pRenderer->Update();

	//�J�����̍X�V����
	m_pCamera->Update();
}

//==============================================================
//�}�l�[�W���̕`�揈��
//==============================================================
void CManager::Draw(void)
{
	//�����_���[�̕`�揈��
	m_pRenderer->Draw();
}

//==============================================================
//�Q�[�����[�h�̐ݒ菈��
//==============================================================
void CManager::SetMode(CScene::MODE mode)
{
	//���݂̃��[�h�̔j��
	if (m_pScene != NULL)
	{
		//�S�Ă̏I������
		CObject::DeathAll();
	}

	//�V�������[�h�̐���
	m_pScene = CScene::Create(mode);

	//����������
	m_pScene->Init();

	//�t�F�[�h�̐�������
	m_pFade = CFade::Create();
}

//==============================================================
//�Q�[�����[�h�̎擾����
//==============================================================
CScene::MODE CManager::GetMode(void)
{
	return m_pScene->GetMode();
}

//==============================================================
//�}�b�v�̔z�u���Z�b�g
//==============================================================
void CManager::ResetMap(void)
{
	//���Z�b�g
	if (m_pInputKeyboard->GetPress(DIK_F2) == true)
	{//F2���������Ƃ�

		//�}�b�v�̃I�u�W�F�N�g�̃��Z�b�g
		CObject::ResetMapAll();

		////�I�u�W�F�N�g3D�̐���
		//CManager::Create3DAll();
	}
}

//==============================================================
//�}�l�[�W���̏��擾
//==============================================================
CManager *CManager::GetInstance(void)
{
	if (m_pManager == NULL)
	{//��������ĂȂ�������

		//����
		return m_pManager = new CManager;
	}
	else
	{//��������Ă���

		return m_pManager;
	}
}

//==============================================================
//FPS�̐ݒ�
//==============================================================
void  CManager::SetFps(int nFps)
{
	m_pRenderer->SetFPS(nFps);		//FPS�̐ݒ�
}