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
		10, // �\����
		10, // ����
		10, // �ڂ�ڂ�
		10 // �\�t�g�N���[��
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
	m_nMapWidth = 1;			// �}�b�v�̉��ԍ�
	m_nMapHeight = 0;			// �}�b�v�̏c�ԍ�
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
CItem* CItem::Create(const TYPE eType, const GRID& pos)
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
	pItem->m_eType = eType;

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

	if (m_bMapScroll == true)
	{
		pos = CMapSystem::GetInstance()->GetGritPos(m_nMapWidth, m_nMapHeight);
		pos.y = 0.0f;
	}
	else
	{
		Move();
	}

	// �v���C���[�ƃA�C�e���̔���
	CollisionPlayer();

	// ���̍X�V
	SetPos(pos);
	SetRot(rot);

	// �e�N���X�̍X�V����
	CObjectX::Update();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG
}

//====================================================================
// �`��
//====================================================================
void CItem::Draw()
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//==========================================
//  �}�b�v�ԍ��̐ݒ�
//==========================================
void CItem::SetGrid(const GRID& pos)
{
	SetWightNumber(pos.x);
	SetHeightNumber(pos.y);
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
		D3DXVECTOR3 pos = player->GetPos();
		D3DXVECTOR3 size = player->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionCircle(posThis, pos, size.x))
		{
			// �擾�ɐ������Ă���ꍇ�X�R�A�����Z����
			if (Hit(player)) { CGame::GetScore()->AddScore(ITEM_SCORE[m_eType]); }
			return true;
		}
	}

	return false;
}
