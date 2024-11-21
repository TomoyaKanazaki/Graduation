//============================================
//
//	�}�l�[�W�� [manager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "Scene.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "logo.h"
#include "camera.h"

#ifdef _DEBUG
#define SET_MODE (CScene::MODE_TITLE)
#else
#define SET_MODE (CScene::MODE_GAME)
#endif // _DEBUG

//�ÓI�����o�ϐ��錾
CScene::MODE CScene::m_mode = SET_MODE;

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
//��������
//====================================================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = nullptr;

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
			pScene = CGame::GetInstance();
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
//����������
//====================================================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CScene::Uninit(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CScene::Update(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CScene::Draw(void)
{

}

//====================================================================
//���[�h�ݒ菈��
//====================================================================
void CScene::SetMode(MODE mode)
{
	CScene* pScene = CManager::GetInstance()->GetScene();
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