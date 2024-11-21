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
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "shadow.h"

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
		200	 // �ڋʏĂ�
	};

	const float BASE_Y = 50.0f; // ����

	const float SHADOW_SIZE = 25.0f;	// �ۉe�̑傫��
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(ITEM_SCORE) == CItem::TYPE_MAX, "ERROR : Type Count Missmatch");

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CItem>* CItem::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority),
m_posBase(INITVECTOR3),
m_fMoveTime(0.0f),
m_pShadow(nullptr)
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

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CItem>* CItem::GetList(void)
{
	return m_pList;
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
	D3DXVECTOR3 pos = GetPos();

	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	if (m_pShadow == nullptr)
	{// �V���h�E����
		m_pShadow = CShadow::Create(pos, SHADOW_SIZE, SHADOW_SIZE);
	}

	pos.y = BASE_Y;
	SetPos(pos);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CItem>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CItem::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�̏I��
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

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
		D3DXVECTOR3 posGrid = m_Grid.ToWorld();
		pos.x = posGrid.x;
		pos.z = posGrid.z;
	}

	// �ړ�����
	Move(pos);

	// �o�ߎ��Ԃ��擾
	m_fMoveTime += DeltaTime::Get();

	// �v���C���[�ƃA�C�e���̔���
	CollisionPlayer();

	if (m_pShadow != nullptr)
	{// �V���h�E�̈ʒu�ݒ�
		m_pShadow->SetPos(D3DXVECTOR3(pos.x, (pos.y - pos.y) + 1.0f, pos.z));
		m_pShadow->SetBaseHeight(pos.y);
	}

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
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �p���N���X�̕`��
	CObjectX::Draw();

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==========================================
//  �}�b�v�ԍ��̐ݒ�
//==========================================
void CItem::SetGrid(const CMapSystem::GRID& pos)
{
	// �O���b�h����ۑ�����
	m_Grid = pos;

	// �O���b�h��񂩂玩�g�̍��W���Z�o����
	D3DXVECTOR3 calc = CMapSystem::GetInstance()->GetGritPos(pos);
	calc.y = BASE_Y;
	m_posBase = calc;

	// �ʒu��ݒ�
	CItem::SetPos(m_posBase);
	CObjectX::SetPos(m_posBase);
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
		// ����ł�ꍇ�͎擾�ł��Ȃ�
		if(player->GetState() == CPlayer::STATE_EGG || player->GetState() == CPlayer::STATE_DEATH)
		{ continue; }

		// �v���C���[�̍��W(�O���b�h�P��)���擾
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// ���ݍ��W����v���Ă��Ȃ��ꍇ���ɐi��
		if (m_Grid != gridPlayer)
		{ continue; }

		// �擾�Ɏ��s�����ꍇ���ɐi��
		if (!Hit(player)) { continue; }

		// �X�R�A�����Z����
		CGame::GetInstance()->GetScore()->AddScore(ITEM_SCORE[m_eType]);
	}

	return false;
}
