//============================================
//
//	�G�̏��� [enemy.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemy.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "objectcharacter.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "slowManager.h"
#include "score.h"
#include "devil.h"
#include "DevilHole.h"
#include "objmeshField.h"
#include "sound.h"
#include "wall.h"
#include "move.h"

#include "enemyBonbon.h"
#include "enemyMedaman.h"
#include "enemyYoungDevil.h"
#include "friedegg.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 60.0f, 35.0f);		//���̓����蔻��
	const float LIFE = 1.0f;	// �̗�
	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�

	const float DISTANCE_RECEDE = 200.0f;	//�߂Â�����
	const float DISTANCE_APPROACH = 100.0f;	//�������鋗��

	//const float COORDDINATE_RATE[] = // �o�H�T�����s���Ԋu
	//{
	//	5.0f,
	//	3.0f,
	//	1.0f
	//};
	//const float TARGET_DIFF = 10.0f; // ���e�͈�
	//const float MOVE_ASTAR = 150.0f; // �ǐՎ��̈ړ����x

	const CMyEffekseer::TYPE EFFECT_TYPE[] = // �o�H�T�����s���Ԋu
	{
		CMyEffekseer::TYPE_RESPAWN_MEDAMAN,
		CMyEffekseer::TYPE_RESPAWN_BONBON,
		CMyEffekseer::TYPE_RESPAWN_BONBON // TODO : �q�f�r��
	};
}

//==========================================
//  �ÓI�x������
//==========================================
//static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(EFFECT_TYPE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CEnemy>* CEnemy::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemy::CEnemy(int nPriority) :CObjectCharacter(nPriority),
m_pEffect(nullptr)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nMoveStateCount = 0;

	m_ColorA = 1.0f;

	m_EnemyType = ENEMY_MEDAMAN;
	m_pSlow = nullptr;

	m_HitState = HIT_STATE_NORMAL;
	m_nHitStateCount = 0;

	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;
	m_Progress.bOKD = true;

	m_Grid.x = 0;
	m_Grid.z = 0;

	m_nBugCounter = 0;

	m_pMoveState = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//��������
//====================================================================
CEnemy* CEnemy::Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid)
{
	// �|�C���^�錾
	CEnemy* pEnemy = nullptr;

	// �^�C�v���Ƃ̃C���X�^���X����
	switch (eType)
	{
	case ENEMY_MEDAMAN: // �߂��܂�
		pEnemy = new CEnemyMedaman;
		break;

	case ENEMY_BONBON: // �ڂ�ڂ�
		pEnemy = new CEnemyBonbon;
		break;

	case ENEMY_LITTLEDEVIL: // ���łт�
		pEnemy = new CEnemyYoungDevil;
		break;

	default:
		assert(false);
		break;
	}

	//����������
	if (FAILED(pEnemy->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// ���W��ݒ�
	pEnemy->SetGrid(grid);
	pEnemy->SetPos(CMapSystem::GetInstance()->GetGritPos(grid));

	// �G�̃^�C�v��ݒ�
	pEnemy->m_EnemyType = eType;

	// �G�t�F�N�g�𐶐�
	pEnemy->Effect();

	return pEnemy;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemy::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_ENEMY3D);

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// �}�g���b�N�X�ݒ�
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());	// �}�b�v�}�g���b�N�X�Ɗ|�����킹
	SetUseStencil(true);	// �X�e���V��

	// �X���[�̐���(�z���A�^�O�̐ݒ�)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	// �ړ���Ԑݒ�
	if (m_pMoveState == nullptr)
	{ // �ړ���Ԑݒ�
		m_pMoveState = new CStateStop();		// ��~���
		m_pMoveState->ControlStop(this);		// ����ł�����
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// �ړ������̏�Ԃ�ݒ�
		m_pMoveState->SetEnemyType(m_EnemyType);					// �G�̎�ނ�ݒ�
	}

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemy>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemy::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// �j��
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CEnemy::Update(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �l���擾
	D3DXVECTOR3 posMy = GetPos();			// �ʒu
	D3DXVECTOR3 posOldMy = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotMy = GetRot();			// ����
	D3DXVECTOR3 sizeMy = GetSize();			// �傫��

	// �ߋ��̈ʒu���L�^
	posOldMy = posMy;
	SetGridOld(m_Grid);		// �O���b�h

	// ��Ԃ̍X�V
	MoveStateManager(posMy);

	// ��Ԃ̍X�V
	HitStateManager(posMy);

	if (m_HitState == HIT_STATE_DEATH)
	{
		return;
	}

	// �ړ�����
	m_pMoveState->Move(this, posMy, rotMy);

	// B�L�[
	if (pInputKeyboard->GetTrigger(DIK_B))
	{
		m_pMoveState->ControlStop(this);			// ��~ or ����
	}
	// N�L�[
	else if (pInputKeyboard->GetTrigger(DIK_N))
	{
		m_pMoveState->ControlAStar(this);			// �ǐ� or ����
		m_pMoveState->SetEnemyType(m_EnemyType);	// �G�̎�ސݒ�

	}
	// M�L�[
	else if (pInputKeyboard->GetTrigger(DIK_M))
	{
		m_pMoveState->RandomAStar(this);			// �����_�� or �ǐ�
		m_pMoveState->SetEnemyType(m_EnemyType);	// �G�̎�ސݒ�
	}

	// �����̔ԍ���ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posMy);

	//���̔���
	if (posMy.y <= 0.0f)
	{
		posMy.y = 0.0f;
		m_move.y = 0.0f;
	}

	// �G�t�F�N�g�𓮂���
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 pos = posMy;
		pos.y += 0.5f;
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotMy, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�G]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�G]���� %f\n", rotMy.y);

	m_pMoveState->Debug();		// ���݂̈ړ����

	// �l�X�V
	SetPos(posMy);			// �ʒu
	SetPosOld(posOldMy);	// �O��̈ʒu
	SetRot(rotMy);			// ����
	SetSize(sizeMy);		// �傫��
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
}

//====================================================================
// �q�b�g����
//====================================================================
bool CEnemy::Hit(void)
{
	Death();

	return true;
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
HRESULT CEnemy::InitModel(const char* pFilename)
{
	// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ���
	CObjectCharacter::SetTxtCharacter(pFilename);
	
	return S_OK;
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ)
{
	// �ǂ̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �ǃ��X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();
		bool bNullJump;

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, size, &bNullJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			SetState(STATE_WAIT);

			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �}�b�v���f���̓����蔻��
//====================================================================
void CEnemy::CollisionDevilHole(useful::COLLISION XYZ)
{
	//// �f�r���z�[���̃��X�g�\����������Δ�����
	//if (CDevilHole::GetList() == nullptr) { return; }
	//std::list<CDevilHole*> list = CDevilHole::GetList()->GetList();    // ���X�g���擾

	//// �f�r���z�[�����X�g�̒��g���m�F����
	//for (CDevilHole* pDevilHole : list)
	//{
	//	D3DXVECTOR3 pos = pDevilHole->GetPos();
	//	D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
	//	D3DXVECTOR3 Size = pDevilHole->GetSize();
	//	bool bNullJump;

	//	// ��`�̓����蔻��
	//	if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
	//	{
	//		//�ҋ@��Ԃɂ���
	//		m_MoveState = MOVE_STATE_WAIT;
	//	}
	//}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CEnemy::CollisionOut(D3DXVECTOR3& posMy)
{
	// //�L���[�u�u���b�N�̃��X�g�\����������Δ�����
	//if (CDevil::GetList() == nullptr) { return; }
	//std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾

	// //�L���[�u�u���b�N���X�g�̒��g���m�F����
	//for (CDevil* pDevil : list)
	//{
	//	D3DXVECTOR3 Pos = pDevil->GetDevilPos();
	//	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	//	float GritSize = CMapSystem::GetInstance()->GetGritSize();

	//	// �X�e�[�W�O�̓����蔻��
	//	if (Pos.x + MapSize.x < posMy.x) // �E
	//	{
	//		posMy.x = Pos.x -MapSize.x - GritSize;
	//	}
	//	if (Pos.x - MapSize.x - GritSize > posMy.x) // ��
	//	{
	//		posMy.x = Pos.x + MapSize.x;
	//	}
	//	if (Pos.z + MapSize.z + GritSize < posMy.z) // ��
	//	{
	//		posMy.z = Pos.z - MapSize.z;
	//	}
	//	if (Pos.z - MapSize.z > posMy.z) // ��
	//	{
	//		posMy.z = Pos.z + MapSize.z + GritSize;
	//	}
	//}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	Uninit();

	// �ڋʏĂ��𐶐�
	CFriedEgg::Create(m_EnemyType, m_Grid);

	SetState(STATE_DEATH);
}

//====================================================================
// ��ԍX�V
//====================================================================
void CEnemy::MoveStateManager(D3DXVECTOR3& posMy)
{
	STATE state = GetState();

	//��Ԃ̊Ǘ�
	switch (state)
	{
	case CObjectCharacter::STATE_WAIT:

		SetState(STATE_WALK);

		break;

	case CObjectCharacter::STATE_TRUN:

		m_move.x = m_move.x * -1.0f;
		m_move.z = m_move.z * -1.0f;

		break;

	case CObjectCharacter::STATE_WALK:

		if (abs(m_move.x) < 0.01f && abs(m_move.z) < 0.01f)
		{
			m_nBugCounter++;

			if (m_nBugCounter > 180)
			{
				posMy = m_Grid.ToWorld();
				m_nBugCounter = 0;
			}
		}

		break;

	case CObjectCharacter::STATE_DEATH:

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		break;

	default:
		break;
	}

	if (m_nMoveStateCount > 0)
	{
		m_nMoveStateCount--;
	}
}

//====================================================================
// ��ԍX�V
//====================================================================
void CEnemy::HitStateManager(D3DXVECTOR3& posMy)
{
	//��Ԃ̊Ǘ�
	switch (m_HitState)
	{
	case CEnemy::HIT_STATE_NORMAL:
		SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case CEnemy::HIT_STATE_DAMAGE:

		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_move = INITVECTOR3;

		if (m_nHitStateCount <= 0)
		{
			m_HitState = HIT_STATE_DEATH;
			Death();
		}

		break;

	case CEnemy::HIT_STATE_DEATH:

		break;

	case CEnemy::HIT_STATE_INVINCIBLE:
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case CEnemy::HIT_STATE_EGG:
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	}

	if (m_nHitStateCount > 0)
	{
		m_nHitStateCount--;
	}
}

//==========================================
//  �G�t�F�N�g�̐���
//==========================================
void CEnemy::Effect()
{
	// �����擾
	D3DXVECTOR3 pos = GetPos();
	pos.y += 0.5f;
	D3DXVECTOR3 rot = GetRot();
	D3DXMATRIX mtx = *GetUseMultiMatrix();
	pos = useful::CalcMatrix(pos, rot, mtx);
	rot = useful::CalcMatrixToRot(mtx);

	// �G�t�F�N�g�𐶐�
	m_pEffect = MyEffekseer::EffectCreate(EFFECT_TYPE[m_EnemyType], false, pos, rot, D3DXVECTOR3(10.0f, 10.0f, 10.0f));
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CEnemy::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
}

//====================================================================
// ���X�g�擾
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
