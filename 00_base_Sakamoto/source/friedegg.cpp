//============================================
//
//	�ڋʏĂ��N���X(friedegg.cpp)
//	Author : Tomoya Kanazaki
//
//============================================
#include "friedegg.h"
#include "XModel.h"
#include "player.h"
#include "manager.h"
#include "sound.h"
#include "move.h"
#include "MapMove.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float DELETE_TIME = 15.0f;	// ���Ŏ���
	const char* MODEL_PASS = "data\\MODEL\\02_Item\\friedegg.x"; // ���f���p�X
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CFriedEgg>* CFriedEgg::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CFriedEgg::CFriedEgg(int nPriority) : CItem(nPriority),
m_fDeleteTimer(0.0f),
m_eCreateType(CEnemy::ENEMY_NONE),
m_pMoveState(nullptr)
{
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CFriedEgg::~CFriedEgg()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CFriedEgg::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_FRIEDEGG);

	// �^�C�v�̐ݒ�
	SetItem(CItem::TYPE_FRIEDEGG);

	// �ړ���Ԑݒ�
	if (m_pMoveState == nullptr)
	{ // �ړ���Ԑݒ�
		m_pMoveState = new CStateRandom();		// ��~���
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// �ړ������̏�Ԃ�ݒ�
	}

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFriedEgg>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CFriedEgg::Uninit(void)
{
	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// �j��
		delete m_pMoveState;
		m_pMoveState = nullptr;
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

	// �e�N���X�̏I������
	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CFriedEgg::Update(void)
{
	// ���Ń^�C�}�[�����Z����
	m_fDeleteTimer += DeltaTime::Get();

	// ���ŏ����𖞂������ꍇ
	if (m_fDeleteTimer >= DELETE_TIME)
	{
		// �G�𐶐�����
		CEnemy::Create(m_eCreateType, GetGrid());

		// ���g���I�����֐��𔲂���
		Uninit();
		return;
	}

	// �e�N���X�̍X�V����
	CItem::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CFriedEgg::Draw(void)
{
	// �e�N���X�̕`�揈��
	CItem::Draw();
}

//====================================================================
//�@�q�b�g����
//====================================================================
bool CFriedEgg::Hit(CPlayer* pPlayer)
{
	// �`��t���O��false�̏ꍇfalse��Ԃ�
	if (!GetDisp()) { return false; }

	// �`����I�t
	SetDisp(false);

	// ���ʉ��̍Đ�
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_EAT);

	// �G�t�F�N�g�𐶐�����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_EAT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// �G�t�F�N�g��ύX����
	ChangeEffect();

	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CFriedEgg>* CFriedEgg::GetList(void)
{
	return m_pList;
}

//==========================================
//  ��������
//==========================================
CFriedEgg* CFriedEgg::Create(const CEnemy::ENEMY_TYPE eType, const CMapSystem::GRID& pos)
{
	// �C���X�^���X����
	CFriedEgg* pItem = new CFriedEgg;

	// ����������
	if (FAILED(pItem->Init())) { assert(false); return nullptr; }

	// �ʒu�̐ݒ�
	pItem->SetGrid(pos);

	// �^�C�v�̐ݒ�
	pItem->SetEnemy(eType);

	// �G�t�F�N�g�𐶐�
	pItem->SetEffect();

	return pItem;
}

//==========================================
//  �ړ�����
//==========================================
void CFriedEgg::Move(D3DXVECTOR3& pos)
{
	// �ړ�����
	m_pMoveState->Move(this, pos, INITVECTOR3);

	// �X�N���[���ɍ��킹�Ĉړ�����
	CMapSystem::GetInstance()->GetMove()->FollowScroll(pos);

	// �����𒲐�����
	pos.y = 50.0f;

	// ���݃O���b�h��ݒ肷��
	SetGrid(CMapSystem::GetInstance()->CalcGrid(pos));
}

//==========================================
//  �G�t�F�N�g�𐶐�
//==========================================
void CFriedEgg::SetEffect()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_STEAM, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
}

//==========================================
//  �G�t�F�N�g��ύX
//==========================================
void CFriedEgg::ChangeEffect()
{
	// ���݂̃G�t�F�N�g���I��
	GetEffect()->SetDeath();

	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	pos.y = 0.0f;
	D3DXVECTOR3 rot = GetRot();

	// �V�����G�t�F�N�g�𐶐�
	switch (m_eCreateType)
	{
	case CEnemy::ENEMY_MEDAMAN:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_MEDAMAN, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	case CEnemy::ENEMY_BONBON:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_BONBON, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	case CEnemy::ENEMY_LITTLEDEVIL:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_SMALLDEVIL, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	default:
		assert(false);
		break;
	}
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CFriedEgg::ChangeMoveState(CMoveState* pMoveState)
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