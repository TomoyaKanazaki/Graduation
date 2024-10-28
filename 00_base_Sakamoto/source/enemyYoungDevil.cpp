//============================================
//
//	�q�f�r���̏��� [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyYoungDevil.h"
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
	const float MOVE_SPEED = 1.0f;
	const float ASHIGARU_SEARCHDISTANCE = 700.0f;	//	���y�̍��G����
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyYoungDevil::CEnemyYoungDevil(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_SWAIT;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyYoungDevil::~CEnemyYoungDevil()
{

}

//====================================================================
//��������
//====================================================================
CEnemyYoungDevil* CEnemyYoungDevil::Create(const char* pFilename)
{
	// ����
	CEnemyYoungDevil* pInstance = new CEnemyYoungDevil();

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
HRESULT CEnemyYoungDevil::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//�G�̎�ސݒ�

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemyYoungDevil::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemyYoungDevil::Uninit(void)
{
	// �p���N���X�̏I������
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemyYoungDevil::Update(void)
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
void CEnemyYoungDevil::TitleUpdate(void)
{
	// �p���N���X�̃^�C�g���X�V����
	CEnemy::TitleUpdate();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemyYoungDevil::GameUpdate(void)
{
	// �p���N���X�̃Q�[���X�V����
	CEnemy::GameUpdate();

	//���[�V�����̊Ǘ�
	ActionState();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyYoungDevil::ActionState(void)
{
	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

		//�ړ����[�V����
	if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
	{
		if (m_Action != ACTION_BMOVE)
		{
			m_Action = ACTION_BMOVE;
			GetMotion()->Set(ACTION_BMOVE, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_SWAIT)
		{
			m_Action = ACTION_SWAIT;
			GetMotion()->Set(ACTION_SWAIT, 5);
		}
	}
}

//====================================================================
//�T����ԊǗ�
//====================================================================
void CEnemyYoungDevil::StateManager(void)
{

}

//====================================================================
//�_���[�W���菈��
//====================================================================
void CEnemyYoungDevil::SetDamage(float Damage)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CEnemyYoungDevil::Draw(void)
{
	// �p���N���X�̕`�揈��
	CEnemy::Draw();
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemyYoungDevil::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_ATTACK)
	{
		m_nStateCount = 1000;
	}
}