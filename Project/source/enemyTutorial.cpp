//============================================
//
//	�`���[�g���A���G�̏��� [enemyTutorial.cpp]
//	Author:���� �쏟
//
//============================================

#include "enemyTutorial.h"

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyTutorial::CEnemyTutorial(int nPriority) : CEnemyAshigaru(nPriority)
{
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyTutorial::~CEnemyTutorial()
{

}

//====================================================================
//��������
//====================================================================
CEnemyTutorial* CEnemyTutorial::Create(const char* pFilename)
{
	// ����
	CEnemyTutorial* pInstance = new CEnemyTutorial();

	// ����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	// ���f���֘A�̏�����
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemyTutorial::Init(void)
{
	// �p���N���X�̏���������
	CEnemyAshigaru::Init();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemyTutorial::Uninit(void)
{
	// �p���N���X�̏I������
	CEnemyAshigaru::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemyTutorial::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CEnemyTutorial::TitleUpdate(void)
{
	// �p���N���X�̃^�C�g���X�V����
	CEnemyAshigaru::TitleUpdate();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemyTutorial::GameUpdate(void)
{
	// �p���N���X�̃Q�[���X�V����
	CEnemyAshigaru::GameUpdate();
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemyTutorial::Draw(void)
{
	// �p���N���X�̕`�揈��
	CEnemyAshigaru::Draw();
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemyTutorial::HitDamage(float fDamage)
{
	CEnemyAshigaru::HitDamage(fDamage);

	if (GetDeath() == true && m_info.bTaskOk == false)
	{
		m_info.bTaskOk = true;
	}
}
