//============================================
//
//	�]�����̏��� [RollRock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RollRock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"
#include "MapSystem.h"
#include "objmeshField.h"
#include "wall.h"
#include "MapMove.h"
#include "move.h"
#include "shadow.h"
#include "player.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		//�����蔻��
	const float GRIT_OK = 45.0f;				//�ړ��\�ȃO���b�g�͈͓̔�
	const float SHADOW_SIZE = 100.0f;			// �ۉe�̑傫��
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRollRock>* CRollRock::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRollRock::CRollRock(int nPriority) : CObjectX(nPriority),
m_pEffect(nullptr),
m_pMoveState(nullptr),
m_pShadow(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//====================================================================
CRollRock::CRollRock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter),
m_SaveGrid(gridCenter),
m_pEffect(nullptr),
m_pMoveState(nullptr),
m_pShadow(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRollRock::~CRollRock()
{

}

//====================================================================
//��������
//====================================================================
CRollRock* CRollRock::Create(CMapSystem::GRID gridCenter)
{
	CRollRock* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CRollRock(3, gridCenter);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init("data\\MODEL\\00_rock.x")))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CRollRock::Init(char* pModelName)
{
	m_pEffect = nullptr;

	// �ʒu
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	pos.y = 50.0f;

	SetType(CObject::TYPE_ROLLROCK);

	CObjectX::Init(pModelName);

	if (m_pShadow == nullptr)
	{// �e����
		m_pShadow = CShadow::Create(pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE));
	}

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// �ʒu�ݒ�
	CObjectX::SetPos(pos);

	// �ړ��ʐݒ�
	CObjectX::SetMove(INITVECTOR3);

	SetSize(SAMPLE_SIZE);

	// �ړ���Ԑݒ�
	if (m_pMoveState == nullptr)
	{ // �ړ���Ԑݒ�
		m_pMoveState = new CStateRoll();		// �]������
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// �ړ������̏�Ԃ�ݒ�
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRollRock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �X�N���[�����I���ɂ���
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRollRock::Uninit(void)
{
	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// �j��
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// �e�̏I��
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	if (m_pList != nullptr)
	{
		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRollRock::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posThis = GetPos();			// �ʒu
	D3DXVECTOR3 posOldThis = GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 rotThis = GetRot();			// ����
	D3DXVECTOR3 sizeThis = GetSize();		// �傫��

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	posOldThis = posThis;
	m_OldGrid = m_Grid;
	CObjectX::SetPosOld(posThis);

	Move(posThis, rotThis);

	// �ړ�������������]
	rotThis.z -= (posThis.x - posOldThis.x) * D3DX_PI * 0.01f;

	// �ړ�������������]
	rotThis.x += (posThis.z - posOldThis.z) * D3DX_PI * 0.01f;

	// �X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut(posThis);

	// �O���b�h����ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posThis);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	if (m_pShadow != nullptr)
	{// �V���h�E�̍X�V
		m_pShadow->SetPos(D3DXVECTOR3(posThis.x, 1.0f, posThis.z));
		//m_pShadow->SetBaseHeight(pos.y);
	}

	// �l�X�V
	SetPos(posThis);		// �ʒu
	SetPosOld(posOldThis);	// �O��̈ʒu
	SetRot(rotThis);		// ����
	SetSize(sizeThis);		// �傫��

	//���_���̍X�V
	CObjectX::Update();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//�`�揈��
//====================================================================
void CRollRock::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
// ���Z�b�g
//====================================================================
void CRollRock::Reset(void)
{
	// �O���b�h�̈ʒu��������
	m_Grid = m_SaveGrid;

	// �ʒu
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	pos.y = 50.0f;

	// �ʒu�ݒ�
	CObjectX::SetPos(pos);

	// �ړ��ʐݒ�
	CObjectX::SetMove(INITVECTOR3);
}

//====================================================================
//�X�����̈ړ�����
//====================================================================
void CRollRock::Move(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// �ړ�����
	m_pMoveState->Move(this, pos, rot);

	D3DXVECTOR3 move = GetMove();	// �ړ���

	// �G�t�F�N�g�̐���
	if (m_pEffect == nullptr && (fabsf(move.x) > 0.1f || fabsf(move.z) > 0.1f))
	{
		D3DXVECTOR3 rotEf = INITVECTOR3;
		rotEf.y = atan2f(-move.x, -move.z);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotEf, mat);
		m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ROLL, true, ef, rotEf);
	}

	// �G�t�F�N�g���ړ�
	if (m_pEffect != nullptr)
	{
		// �v�Z�Ɏg�p����l�̎擾
		D3DXMATRIX mat = *GetUseMultiMatrix();

		// ���W�ƌ����Ƀ}�g���b�N�X�𔽉f
		D3DXVECTOR3 posEf = useful::CalcMatrix(pos, rot, mat);

		// �G�t�F�N�g�ɏ���K�p
		m_pEffect->SetPosition(posEf);
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr && fabsf(move.x) <= 0.1f && fabsf(move.z) <= 0.1f)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CRollRock::CollisionOut(D3DXVECTOR3& pos)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 Pos = pDevil->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize();

		// �X�e�[�W�O�̓����蔻��
		if (Pos.x + MapSize.x < pos.x) // �E
		{
			pos.x = Pos.x - MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > pos.x) // ��
		{
			pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < pos.z) // ��
		{
			pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > pos.z) // ��
		{
			pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//==========================================
//  �v���C���[�Ƃ̓����蔻��
//==========================================
void CRollRock::CollisionPlayer()
{
	// �O��̃O���b�h�ƍ���̃O���b�h����v���Ă���ꍇ�֐��𔲂���
	if (m_Grid == m_OldGrid) { return; }

	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // ���X�g���擾

	// �v���C���[���X�g�̒��g���m�F����
	for (CPlayer* player : list)
	{
		// ����ł�ꍇ�͎擾�ł��Ȃ�
		if (player->GetState() == CPlayer::STATE_EGG || player->GetState() == CPlayer::STATE_DEATH)
		{
			continue;
		}

		// �v���C���[�̍��W(�O���b�h�P��)���擾
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// ���ݍ��W����v���Ă����ꍇ�E��
		if (m_Grid == gridPlayer)
		{
			player->Death();
		}
	}

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// �}�b�v�����擾
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// �O��̃O���b�h���ړ��\�n�_�ɐݒ�
	generator->removeCollision(m_OldGrid.ToAStar());
	pMapSystem->SetGritBool(m_OldGrid, false);

	// ���݂̃O���b�h���ړ��s�n�_�ɐݒ�
	generator->addCollision(m_Grid.ToAStar());
	pMapSystem->SetGritBool(m_Grid, true);
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CRollRock::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}