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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\02_item\\holybible.x"; // ���f���p�X
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
	m_fMove = 0.0f;
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
	// �p���N���X�̏�����
	CItem::Init(MODEL_PASS);

	SetType(CObject::TYPE_BIBLE);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �X�N���[���̑Ώۂ���O��
	SetMapScroll(false);

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
// �^�C�g���ł̍X�V
//====================================================================
void CBible::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	//�ʒu�X�V
	pos += move;

	SetPos(pos);

	//���_���̍X�V
	CItem::Update();
}

//====================================================================
// �Q�[���ł̍X�V
//====================================================================
void CBible::GameUpdate(void)
{
	//��ԊǗ�
	StateManager();

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
	D3DXVECTOR3 pos = GetPos();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG

	//�ʒu�X�V
	CItem::SetPos(pos);
	CObjectX::SetPos(pos);
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

//====================================================================
//��ԊǗ�
//====================================================================
void CBible::StateManager(void)
{
	CItem::STATE State = GetState();

	int nStateCounter = GetStateCounter();

	switch (State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (nStateCounter > 0)
	{
		nStateCounter--;
	}
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBible>* CBible::GetList(void)
{
	return m_pList;
}
