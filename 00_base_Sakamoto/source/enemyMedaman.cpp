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
#include "character.h"
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

	const char* MODEL_PASS = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt"; // ���f���p�X
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CEnemyMedaman>* CEnemyMedaman::m_pList = nullptr; // �I�u�W�F�N�g���X�g

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
//����������
//====================================================================
HRESULT CEnemyMedaman::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//�G�̎�ސݒ�

	// �����ڂ̐ݒ�
	InitModel(MODEL_PASS);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemyMedaman>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemyMedaman::Uninit(void)
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
void CEnemyMedaman::Update(void)
{
	//���[�V�����̊Ǘ�
	ActionState();

	// �e�X�V
	CEnemy::Update();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyMedaman::ActionState(void)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

		//�ړ����[�V����
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, MOTION_BLEND_NEUTRAL);
		}
	}
	//�j���[�g�������[�V����
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

	if (m_Action != ACTION_TEST)
	{
		m_nStateCount = 1000;
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CEnemyMedaman>* CEnemyMedaman::GetList(void)
{
	return m_pList;
}
