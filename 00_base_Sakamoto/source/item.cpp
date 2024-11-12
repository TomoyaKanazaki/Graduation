//============================================
//
//	�A�C�e���̏��� [item.cpp]
//	Author:morikawa shunya
//
//============================================
#include "item.h"
#include "cross.h"
#include "bowabowa.h"
#include "player.h"
#include "useful.h"
#include "effect.h"
#include "MapSystem.h"
#include "bible.h"
#include "game.h"
#include "score.h"
#include "softcream.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	// ���Z����X�R�A
	const int ITEM_SCORE[] =
	{
		100, // �\����
		100, // ����
		100, // �ڂ�ڂ�
		400, // �\�t�g�N���[��
		200 // �ڋʏĂ�
	};
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(ITEM_SCORE) == CItem::TYPE_MAX, "ERROR : Type Count Missmatch");

//====================================================================
// �R���X�g���N�^
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	m_eType = TYPE_NONE;		// ���
	m_nIdxXModel = 0;			// X���f���ԍ�
	m_nIdxTexture = -1;			// �e�N�X�`���ԍ�
	m_nStateCount = 0;			// ��ԊǗ�
	m_fColorA = 0.0f;			// �s�����x
	m_Scaling = 0.0f;			// �傫��
	m_bCollision = false;		// �����蔻��
	m_CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ������W
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_Grid.x = 1;			// �}�b�v�̉��ԍ�
	m_Grid.z = 0;			// �}�b�v�̏c�ԍ�
	m_bMapScroll = true;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CItem::~CItem()
{

}

//====================================================================
// ����
//====================================================================
CItem* CItem::Create(const TYPE eType, const CMapSystem::GRID& pos)
{
	// �C���X�^���X����
	CItem* pItem = nullptr;

	if (pItem == nullptr)
	{
		switch (eType)
		{
		case CItem::TYPE_CROSS:		// �\����
			pItem = new CCross;
			break;

		case CItem::TYPE_BIBLE:		// ����
			 pItem = new CBible;
			break;

		case CItem::TYPE_BOWABOWA:	// �ڂ�ڂ�
			pItem = new CBowabowa;
			break;

		case CItem::TYPE_SOFTCREAM:	// �\�t�g�N���[��
			pItem = new CSoftCream;
			break;

		case CItem::TYPE_FRIEDEGG: // �ڋʏĂ�
			// �K���I�[�o�[���C�h����Ăяo������
			assert(false);
			break;

		default:
			assert(false);
			break;
		}
	}

	// ����������
	pItem->Init();

	// �ʒu�̐ݒ�
	pItem->SetGrid(pos);

	// �^�C�v�̐ݒ�
	pItem->SetItem(eType);

	return pItem;
}

//====================================================================
// ������
//====================================================================
HRESULT CItem::Init(const char* pModelName)
{
	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

	D3DXVECTOR3 pos = GetPos();
	pos.y = 50.0f;
	SetPos(pos);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CItem::Uninit()
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CItem::Update()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (m_bMapScroll)
	{
		// �O���b�h�ɉ��������W���擾�A�ݒ�
		D3DXVECTOR3 posGrid = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		pos.x = posGrid.x;
		pos.z = posGrid.z;
	}

	// �ړ�����
	Move(pos);

	// �v���C���[�ƃA�C�e���̔���
	CollisionPlayer();

	// ���̍X�V
	SetPos(pos);
	SetRot(rot);
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);

	// �e�N���X�̍X�V����
	CObjectX::Update();
}

//====================================================================
// �`��
//====================================================================
void CItem::Draw()
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//====================================================================
// �v���C���[�Ƃ̓����蔻��
//====================================================================
bool CItem::CollisionPlayer()
{
	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { return false; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // ���X�g���擾

	// ���g�̍��W���擾����
	D3DXVECTOR3 posThis = GetPos();

	// �v���C���[���X�g�̒��g���m�F����
	for (CPlayer* player : list)
	{
		// �v���C���[�̍��W(�O���b�h�P��)���擾
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// ���ݍ��W����v���Ă��Ȃ��ꍇ���ɐi��
		if (m_Grid != gridPlayer)
		{ continue; }

		// �擾�Ɏ��s�����ꍇ���ɐi��
		if (!Hit(player)) { continue; }

		// �X�R�A�����Z����
		CGame::GetScore()->AddScore(ITEM_SCORE[m_eType]);
	}

	return false;
}
