//============================================
//
//	�{���{���̏��� [enemyBonbon.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyBonbon.h"
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

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CEnemyBonbon>* CEnemyBonbon::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyBonbon::CEnemyBonbon(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_NEUTRAL;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyBonbon::~CEnemyBonbon()
{

}

//====================================================================
//��������
//====================================================================
CEnemyBonbon* CEnemyBonbon::Create(const char* pFilename)
{
	// ����
	CEnemyBonbon* pInstance = new CEnemyBonbon();

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
HRESULT CEnemyBonbon::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//�G�̎�ސݒ�

		// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemyBonbon>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemyBonbon::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �p���N���X�̏I������
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemyBonbon::Update(void)
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
void CEnemyBonbon::TitleUpdate(void)
{
	// �p���N���X�̃^�C�g���X�V����
	CEnemy::TitleUpdate();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemyBonbon::GameUpdate(void)
{
	// �p���N���X�̃Q�[���X�V����
	CEnemy::GameUpdate();

	//���[�V�����̊Ǘ�
	ActionState();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyBonbon::ActionState(void)
{
	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

	// �ړ����[�V����
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			GetMotion()->Set(ACTION_MOVE, MOTION_BLEND_NEUTRAL);
		}
	}
	// �j���[�g�������[�V����
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
void CEnemyBonbon::StateManager(void)
{

}

//====================================================================
//�_���[�W���菈��
//====================================================================
void CEnemyBonbon::SetDamage(float Damage)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CEnemyBonbon::Draw(void)
{
	// �p���N���X�̕`�揈��
	CEnemy::Draw();
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemyBonbon::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_TEST)
	{
		m_nStateCount = 1000;
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CEnemyBonbon>* CEnemyBonbon::GetList(void)
{
	return m_pList;
}
