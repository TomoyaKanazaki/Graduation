//============================================
//
//	�����̏��� [bible.cpp]
//	Author:morikawa shunya
//
//============================================
#include "bible.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\02_item\\holybible.x"; // ���f���p�X
	const float MOVE_SCALE = sqrtf(50.0f * 50.0f * 2.0f); // �ړ���
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CBible>* CBible::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority),
m_posBase(INITVECTOR3)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// ������
//====================================================================
HRESULT CBible::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_BIBLE);

	// �X�N���[���̑Ώۂ���O��
	SetMapScroll(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CBible::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �p���N���X�̏I��
	CItem::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CBible::Update(void)
{
	//�e�N���X�̍X�V
	CItem::Update();
}

//====================================================================
// �`��
//====================================================================
void CBible::Draw(void)
{
	// �p���N���X�̕`��
	CItem::Draw();
}

//====================================================================
// �����̐���
//====================================================================
void CBible::Move()
{
	// �t�B�[���h�̍��W���擾
	D3DXVECTOR3 posField = CGame::GetMapField()->GetPos();

	// ���g�̍��W���擾 
	D3DXVECTOR3 posThis = GetPos();

	// ���g�̍��W�ƃt�B�[���h�����Ԏ��̌������Z�o
	D3DXVECTOR3 vec = posThis - posField;
	float rot = atan2f(vec.z, vec.x);

	// �o�ߎ��Ԃ��擾
	m_fMoveTime += DeltaTime::Get();

	// �ړ����Ɍo�ߎ��Ԃ��������킹��
	float fScale = sinf(m_fMoveTime) * MOVE_SCALE;

	// �ړ�����xz�����ɕ�������
	float x = fScale * cosf(rot);
	float z = fScale * sinf(rot);

	// ��ʒu�Ɉړ��ʂ����Z����
	posThis.x = m_posBase.x + x;
	posThis.z = m_posBase.z + z;
}

//==========================================
//  �}�b�v�ԍ��̐ݒ�
//==========================================
void CBible::SetGrid(const GRID& pos)
{
	// �e�N���X�̐ݒ菈�����Ăяo��
	CItem::SetGrid(pos);

	// �O���b�h��񂩂玩�g�̍��W���Z�o����
	m_posBase = CMapSystem::GetInstance()->GetGritPos(pos.x, pos.y);

	// �ʒu��ݒ�
	CItem::SetPos(m_posBase);
	CObjectX::SetPos(m_posBase);
}

//====================================================================
// ��ԊǗ�
//====================================================================
bool CBible::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
	{
		return false;
	}

	// �v���C���[�̃A�C�e����ݒ�
	pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

	// ���g�̍폜
	Uninit();
	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBible>* CBible::GetList(void)
{
	return m_pList;
}
