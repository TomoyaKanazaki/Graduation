//============================================
//
//	���_�}���̏��� [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyMedaman.h"
#include "LevelModelEffect.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "effect.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float MOTION_MOVE = 0.1f;		// �ړ���ԃ��[�V�����ύX�ւ̈ړ���

	const int MOTION_BLEND_NEUTRAL = 5;	// �ҋ@��Ԃ̃��[�V�����u�����h����
	const int MOTION_BLEND_MOVE = 5;	// �ړ���Ԃ̃��[�V�����u�����h����
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyMedaman::CEnemyMedaman(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_NEUTRAL;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyMedaman::~CEnemyMedaman()
{

}

//====================================================================
//��������
//====================================================================
CEnemyMedaman* CEnemyMedaman::Create(const char* pFilename)
{
	// ����
	CEnemyMedaman* pInstance = new CEnemyMedaman();

	// ����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// ���f���֘A�̏�����
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemyMedaman::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//�G�̎�ސݒ�

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemyMedaman::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemyMedaman::Uninit(void)
{
	// �p���N���X�̏I������
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemyMedaman::Update(void)
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
void CEnemyMedaman::TitleUpdate(void)
{
	// �p���N���X�̃^�C�g���X�V����
	CEnemy::TitleUpdate();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemyMedaman::GameUpdate(void)
{
	// �p���N���X�̃Q�[���X�V����
	CEnemy::GameUpdate();

	//���[�V�����̊Ǘ�
	ActionState();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyMedaman::ActionState(void)
{
	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

		//�ړ����[�V����
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			GetMotion()->Set(ACTION_MOVE, MOTION_BLEND_NEUTRAL);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			GetMotion()->Set(ACTION_NEUTRAL, MOTION_BLEND_MOVE);
		}
	}
}

//====================================================================
//�T����ԊǗ�
//====================================================================
void CEnemyMedaman::StateManager(void)
{

}

//====================================================================
//�_���[�W���菈��
//====================================================================
void CEnemyMedaman::SetDamage(float Damage)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CEnemyMedaman::Draw(void)
{
	// �p���N���X�̕`�揈��
	CEnemy::Draw();
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemyMedaman::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_TEST)
	{
		m_nStateCount = 1000;
	}
}