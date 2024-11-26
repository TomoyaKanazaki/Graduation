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
#include "select.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "camera.h"

//�ÓI�����o�ϐ��錾
CScene::MODE CScene::m_mode = MODE_TITLE;

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
		case MODE_TITLE:
			pScene = CTitle::GetInstance();
			break;
		case MODE_SELECT:
			pScene = CSelect::GetInstance();
			break;
		case MODE_GAME:
			pScene = CGame::GetInstance();
			break;
		case MODE_RESULT:
			pScene = CResult::GetInstance();
			break;
		case CScene::MODE_TUTORIAL:
			pScene = CTutorial::GetInstance();
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