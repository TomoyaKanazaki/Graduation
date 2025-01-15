//============================================
//
//	�q�f�r���̏��� [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyYoungDevil.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "objectcharacter.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float MOTION_MOVE = 0.1f;		// �ړ���ԃ��[�V�����ύX�ւ̈ړ���

	const int MOTION_BLEND_NEUTRAL = 5;	// �ҋ@��Ԃ̃��[�V�����u�����h����
	const int MOTION_BLEND_MOVE = 5;	// �ړ���Ԃ̃��[�V�����u�����h����

	const int MODEL_REF = 101;	// �X�e���V���Q�ƒl

	const char* MODEL_PASS = "data\\TXT\\MOTION\\01_enemy\\motion_smalldevil.txt"; // ���f���p�X
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CEnemyYoungDevil>* CEnemyYoungDevil::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyYoungDevil::CEnemyYoungDevil(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_NEUTRAL;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyYoungDevil::~CEnemyYoungDevil()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CEnemyYoungDevil::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_LITTLEDEVIL);	//�G�̎�ސݒ�

	// �����ڂ̐ݒ�
	InitModel(MODEL_PASS, MODEL_REF);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemyYoungDevil>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemyYoungDevil::Uninit(void)
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
void CEnemyYoungDevil::Update(void)
{
	//���[�V�����̊Ǘ�
	ActionState();

	// �e�X�V
	CEnemy::Update();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyYoungDevil::ActionState(void)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

	// �ړ����[�V����
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, MOTION_BLEND_NEUTRAL);
		}
	}
	// �j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			pMotion->Set(ACTION_NEUTRAL, MOTION_BLEND_MOVE);
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
bool CEnemyYoungDevil::Hit(void)
{
	return true;
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CEnemyYoungDevil>* CEnemyYoungDevil::GetList(void)
{
	return m_pList;
}
